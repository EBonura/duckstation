#include "bridge_memory_registry.h"

#include <cstring>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace BridgeMemoryRegistry {

static std::mutex s_mutex;
static std::unordered_map<std::string, std::vector<uint8_t>> s_bios_blobs;
static std::unordered_map<std::string, std::vector<uint8_t>> s_disc_blobs;
static std::unordered_map<std::string, std::string> s_disc_formats;
static std::unordered_map<std::string, std::string> s_cue_texts;
static std::unordered_map<std::string, std::unordered_map<std::string, std::vector<uint8_t>>> s_cue_files;

bool IsRegisteredURI(const char* path)
{
  return path && std::strncmp(path, URI_PREFIX, URI_PREFIX_LEN) == 0;
}

void RegisterBios(const std::string& uri, const uint8_t* data, size_t len)
{
  std::lock_guard lock(s_mutex);
  auto& blob = s_bios_blobs[uri];
  blob.assign(data, data + len);
}

std::span<const uint8_t> LookupBios(const std::string& uri)
{
  std::lock_guard lock(s_mutex);
  auto it = s_bios_blobs.find(uri);
  if (it == s_bios_blobs.end())
    return {};
  return std::span<const uint8_t>(it->second);
}

void RegisterDisc(const std::string& uri, const uint8_t* data, size_t len, const std::string& format)
{
  std::lock_guard lock(s_mutex);
  auto& blob = s_disc_blobs[uri];
  blob.assign(data, data + len);
  s_disc_formats[uri] = format;
  s_cue_texts.erase(uri);
  s_cue_files.erase(uri);
}

std::span<const uint8_t> LookupDisc(const std::string& uri)
{
  std::lock_guard lock(s_mutex);
  auto it = s_disc_blobs.find(uri);
  if (it == s_disc_blobs.end())
    return {};
  return std::span<const uint8_t>(it->second);
}

std::string LookupDiscFormat(const std::string& uri)
{
  std::lock_guard lock(s_mutex);
  auto it = s_disc_formats.find(uri);
  if (it == s_disc_formats.end())
    return {};
  return it->second;
}

void RegisterCueDisc(const std::string& uri, const std::string& cue_text)
{
  std::lock_guard lock(s_mutex);
  s_disc_blobs.erase(uri);
  s_disc_formats[uri] = "cue";
  s_cue_texts[uri] = cue_text;
  s_cue_files[uri].clear();
}

void RegisterCueDiscFile(const std::string& uri, const std::string& filename, const uint8_t* data, size_t len)
{
  std::lock_guard lock(s_mutex);
  auto& files = s_cue_files[uri];
  auto& blob = files[filename];
  blob.assign(data, data + len);
}

std::string LookupCueDiscText(const std::string& uri)
{
  std::lock_guard lock(s_mutex);
  auto it = s_cue_texts.find(uri);
  if (it == s_cue_texts.end())
    return {};
  return it->second;
}

std::vector<std::pair<std::string, std::vector<uint8_t>>> LookupCueDiscFiles(const std::string& uri)
{
  std::lock_guard lock(s_mutex);
  std::vector<std::pair<std::string, std::vector<uint8_t>>> files;
  auto it = s_cue_files.find(uri);
  if (it == s_cue_files.end())
    return files;
  files.reserve(it->second.size());
  for (const auto& [name, data] : it->second)
    files.emplace_back(name, data);
  return files;
}

void Clear()
{
  std::lock_guard lock(s_mutex);
  s_bios_blobs.clear();
  s_disc_blobs.clear();
  s_disc_formats.clear();
  s_cue_texts.clear();
  s_cue_files.clear();
}

} // namespace BridgeMemoryRegistry
