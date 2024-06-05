#include <algorithm>
#include <string>
#include <vector>

#include "header/kvstore.h"
#include "header/Level.h"
#include "header/file_ops.h"
#include "header/utils.h"


void KVStore::saveMemTable() {
  if (memTable.itemCnt() == 0) return;
  maxFileNo++;
  if (!utils::dirExists(dir + "/level-0")) {
    utils::mkdir((dir + "/level-0").c_str());
    levels.emplace_back(0, config.getLayerSize(0), config.getLayerType(0));
  }
  std::string filename = dir + "/level-0/" + std::to_string(maxFileNo) + ".sst";
  auto sstable = memTable.toSSTable(++maxTimeStamp);
  sstable.toFile(filename);
  sstable.freeData();
  levels[0].addNewSSTable(std::move(sstable));
  memTable.clear();
  if (levels[0].overSized()) compact();
}

KVStore::KVStore(const std::string &dir) : KVStoreAPI(dir), dir(dir) {
  config.loadConfig(lsmkv_constants::CONFIG_FILENAME);
  if (utils::dirExists(dir)) {
    std::vector<std::string> paths;
    utils::scanDir(dir, paths);
    for (const auto &path : paths) {
      std::string levelPath = dir + "/" + path;
      if (!file_ops::isDir(levelPath) || path.substr(0, 6) != "level-") {
        throw std::runtime_error("Unexpected file in kvstore directory");
      }
      auto level_id = std::stoul(path.substr(6));
      if (level_id >= levels.size()) {
        for (auto i = levels.size(); i <= level_id; i++) {
          levels.push_back(
              Level(i, config.getLayerSize(i), config.getLayerType(i)));
        }
      }
      std::vector<std::string> level_filenames;
      utils::scanDir(levelPath, level_filenames);
      for (const auto &sstFilename : level_filenames) {
        std::string file_path = levelPath + "/" + sstFilename;
        if (!file_ops::isFile(file_path) ||
            sstFilename.substr(sstFilename.length() - 4) != ".sst") {
          throw std::runtime_error(
              "Unexpected file in kvstore level directory");
        }
        this->maxFileNo = std::max(
            this->maxFileNo,
            std::stoul(sstFilename.substr(0, sstFilename.length() - 4)));
      }
    }
  } else {
    utils::mkdir(dir.c_str());
  }
}

KVStore::~KVStore() { saveMemTable(); }


std::string KVStore::get(uint64_t key) {
  auto value = memTable.get(key);
  if (value == "~DELETED~") return "";
  if (value != "") return value;
  for (size_t levelId = 0; levelId < levels.size(); levelId++) {
    std::string value;
    if (levels[levelId].find(key, value)) {
      if (value == "~DELETED~") return "";
      return value;
    }
  }
  return "";
}


void KVStore::scan(uint64_t key1, uint64_t key2,std::list<std::pair<uint64_t, std::string> > &list) {}

void KVStore::compact() {
  uint64_t currentLevel = 0;
  while (1) {
    if (!levels[currentLevel].overSized()) break;
    if (levels.size() == currentLevel + 1ul) {
      if (!utils::dirExists(dir + "/level-" +
                            std::to_string(currentLevel + 1))) {
        utils::mkdir(
            (dir + "/level-" + std::to_string(currentLevel + 1)).c_str());
      }
      levels.emplace_back(currentLevel + 1,
                          config.getLayerSize(currentLevel + 1),
                          config.getLayerType(currentLevel + 1));
    }
    levels[currentLevel].compactInto(levels[currentLevel + 1], maxFileNo, dir);
    currentLevel++;
  }
}

void KVStore::putDiag(uint64_t key, const std::string &s,bool &compact) {
  compact=false;
  bool memTablePutSuccessful = memTable.put(key, s);
  if (!memTablePutSuccessful) {
    compact=true;
    saveMemTable();
    memTable.clear();
    memTable.put(key, s);
  }
}


std::string KVStore::getDiag(uint64_t key, bool useCache, bool useBloomfilter) {
  auto value = memTable.get(key);
  if (value == "~DELETED~") return "";
  if (value != "") return value;
  for (size_t levelId = 0; levelId < levels.size(); levelId++) {
    std::string value;
    if (levels[levelId].find(key, value, useCache, useBloomfilter)) {
      if (value == "~DELETED~") return "";
      return value;
    }
  }
  return "";
}


void KVStore::put(uint64_t key, const std::string &s) {
  bool memTablePutSuccessful = memTable.put(key, s);
  if (!memTablePutSuccessful) {
    saveMemTable();
    memTable.clear();
    memTable.put(key, s);
  }
}


void KVStore::reset() {
  memTable.clear();
  levels.clear();
  maxFileNo = 0;
  maxTimeStamp = 0;
  std::vector<std::string> paths;
  utils::scanDir(dir, paths);
  for (const auto &path : paths) {
    std::string levelPath = dir + "/" + path;
    if (!file_ops::isDir(levelPath) || path.substr(0, 6) != "level-") {
      throw std::runtime_error("Unexpected file in kvstore directory");
    }
    std::vector<std::string> level_filenames;
    utils::scanDir(levelPath, level_filenames);
    for (const auto &sstFilename : level_filenames) {
      std::string file_path = levelPath + "/" + sstFilename;
      if (!file_ops::isFile(file_path) ||
          sstFilename.substr(sstFilename.length() - 4) != ".sst") {
        throw std::runtime_error("Unexpected file in kvstore level directory");
      }
      utils::rmfile(file_path.c_str());
    }
    utils::rmdir(levelPath.c_str());
  }
}

bool KVStore::del(uint64_t key) {
  if (this->get(key) == "") return false;
  this->put(key, "~DELETED~");
  return true;
}
