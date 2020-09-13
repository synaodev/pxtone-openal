#include "./al-check.hpp"

#if defined(LEVIATHAN_TOOLCHAIN_MSVC) && !defined(_CRT_SECURE_NO_WARNINGS)
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstring>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <thread>

#include <fmt/format.h>
#include <pxtone/pxtnService.h>
#include <pxtone/pxtnError.h>

constexpr size_t  kLargestTune  = 10485760;
constexpr float   kBufferedTime = 0.1f;
constexpr float   kWaitConstant = 750.0f;
constexpr int32_t kAudioBitrate = pxtnBITPERSAMPLE / 8;

std::vector<char> load_tune(const std::string& path) {
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs.is_open()) {
		return {};
	}
	ifs.seekg(0, std::ios::end);
	size_t length = static_cast<size_t>(ifs.tellg());
	if (length == 0) {
		return {};
	}
	ifs.seekg(0, std::ios::beg);
	std::vector<char> result;
	result.resize(length);
	ifs.read(reinterpret_cast<char*>(&result[0]), length);
	return result;
}

bool play_tune(pxtnService& service, uint32_t& source, std::array<uint32_t, 4>& buffers) {
	// Initialize Necessary Data
	bool playing = true;
	int32_t amount = static_cast<int32_t>(kBufferedTime * static_cast<float>(2 * 44100 * kAudioBitrate));
	std::vector<char> vector = std::vector<char>(static_cast<size_t>(amount), 0);
	if (vector.empty()) {
		return false;
	}
	// Queue Tune Beginning
	int32_t state = 0;
	int32_t procs = 0;
	for (auto&& buffer : buffers) {
		if (service.Moo(&vector[0], amount)) {
			alCheck(alBufferData(
				buffer,
				AL_FORMAT_STEREO16,
				&vector[0],
				amount,
				44100
			));
			alCheck(alSourceQueueBuffers(source, 1, &buffer));
		} else {
			playing = false;
		}
	}
	// Main Loop
	while (playing) {
		alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));
		if (state != AL_PLAYING) {
			alCheck(alSourcePlay(source));
		}
		alCheck(alGetSourcei(source, AL_BUFFERS_PROCESSED, &procs));
		while (procs > 0) {
			uint32_t buffer = 0;
			alCheck(alSourceUnqueueBuffers(source, 1, &buffer));
			if (service.Moo(&vector[0], amount)) {
				alCheck(alBufferData(
					buffer,
					AL_FORMAT_STEREO16,
					&vector[0],
					amount,
					44100
				));
				alCheck(alSourceQueueBuffers(source, 1, &buffer));
				procs--;
			} else {
				playing = false;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	// Clean Up
	alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));
	if (state == AL_PLAYING) {
		alCheck(alSourceStop(source));
	}
	alCheck(alSourcei(source, AL_BUFFER, 0));
	return true;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		return -1;
	}
	bool error = false;
	bool clear = false;
	const char* path = argv[1];
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
	pxtnService service;
	pxtnERR result = pxtnOK;
	uint32_t source = 0;
	std::array<uint32_t, 4> buffers;
	buffers.fill(0);
	pxtnVOMITPREPARATION preparation;
	std::memset(&preparation, 0, sizeof(pxtnVOMITPREPARATION));
	// Initialize
	device = alcOpenDevice(nullptr);
	if (device == nullptr) {
		error = true;
		goto cleanup;
	}
	context = alcCreateContext(device, nullptr);
	if (context == nullptr) {
		error = true;
		goto cleanup;
	}
	if (alcMakeContextCurrent(context) == 0) {
		error = true;
		goto cleanup;
	}
	result = service.init();
	if (result != pxtnOK) {
		error = true;
		goto cleanup;
	}
	if (!service.set_destination_quality(2, 44100)) {
		error = true;
		goto cleanup;
	}
	alCheck(alGenBuffers(buffers.size(), &buffers[0]));
	if (buffers[0] == 0) {
		error = true;
		goto cleanup;
	}
	alCheck(alGenSources(1, &source));
	if (source == 0) {
		error = true;
		goto cleanup;
	}
	// Load Tune
	{
		std::vector<char> tune = load_tune(path);
		if (tune.empty()) {
			error = true;
			goto cleanup;
		}
		size_t length = tune.size();
		if (length >= kLargestTune) {
			error = true;
			goto cleanup;
		}
		pxtnDescriptor descriptor;
		if (!descriptor.set_memory_r(&tune[0], static_cast<int>(length))) {
			error = true;
			goto cleanup;
		}
		clear = true;
		result = service.read(&descriptor);
		if (result != pxtnOK) {
			error = true;
			goto cleanup;
		}
		result = service.tones_ready();
		if (result != pxtnOK) {
			error = true;
			goto cleanup;
		}
	}
	// Prep
	preparation.flags = 0;
	preparation.start_pos_float = 0.0f;
	preparation.fadein_sec = 0.0f;
	preparation.master_volume = 1.0f;
	if (!service.moo_preparation(&preparation)) {
		error = true;
		goto cleanup;
	}
	// Loop
	if (!play_tune(service, source, buffers)) {
		error = true;
		goto cleanup;
	}
cleanup:
	if (clear) {
		service.clear();
		clear = false;
	}
	if (source != 0) {
		int32_t state = 0;
		alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));
		if (state == AL_PLAYING) {
			alCheck(alSourceStop(source));
		}
		alCheck(alSourcei(source, AL_BUFFER, 0));
		alCheck(alDeleteSources(1, &source));
		source = 0;
	}
	if (buffers[0] != 0) {
		alCheck(alDeleteBuffers(buffers.size(), &buffers[0]));
		buffers.fill(0);
	}
	if (result != pxtnOK) {
		fmt::print("Pxtone Error: {}\n", pxtnError_get_string(result));
		result = pxtnOK;
	}
	if (context != nullptr) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		context = nullptr;
	}
	if (device != nullptr) {
		alcCloseDevice(device);
		device = nullptr;
	}
	return error ? -1 : 0;
}
