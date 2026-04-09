#pragma once
#include <cstddef>
#include <cstdint>
#include <utility>
#include <span>
#include <string>
#include <vector>

namespace BridgeMemoryRegistry {

// URI prefix for bridge-registered blobs
static constexpr const char* URI_PREFIX = "psoxide-mem://";
static constexpr size_t URI_PREFIX_LEN = 14; // strlen("psoxide-mem://")

bool IsRegisteredURI(const char* path);

// BIOS registry
void RegisterBios(const std::string& uri, const uint8_t* data, size_t len);
std::span<const uint8_t> LookupBios(const std::string& uri);

// Disc registry — stores raw disc bytes and format hint
void RegisterDisc(const std::string& uri, const uint8_t* data, size_t len, const std::string& format);
std::span<const uint8_t> LookupDisc(const std::string& uri);
std::string LookupDiscFormat(const std::string& uri);
void RegisterCueDisc(const std::string& uri, const std::string& cue_text);
void RegisterCueDiscFile(const std::string& uri, const std::string& filename, const uint8_t* data, size_t len);
std::string LookupCueDiscText(const std::string& uri);
std::vector<std::pair<std::string, std::vector<uint8_t>>> LookupCueDiscFiles(const std::string& uri);

// Cleanup
void Clear();

} // namespace BridgeMemoryRegistry
