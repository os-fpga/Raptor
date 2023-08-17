#include "CFGDeviceDatabase.h"

#include "CFGCommonRS/CFGCommonRS.h"
#include "CFGObject/CFGObject_auto.h"

struct DEVICE_INFO {
  std::string name = "";
  std::string family = "";
  int findex = 0;
  std::string series = "";
  int sindex = 0;
  std::string protocol = "";
  int pindex = 0;
  std::string blwl = "";
  int bindex = 0;
};

static void get_device(std::string name, nlohmann::json& json,
                       std::vector<DEVICE_INFO*>& devices) {
  DEVICE_INFO* device = CFG_MEM_NEW(DEVICE_INFO);
  device->name = name;
  CFG_ASSERT(device->name.size());
  CFG_ASSERT(json.is_object());
  // family
  CFG_ASSERT(json.contains("family"));
  CFG_ASSERT(json["family"].is_string());
  device->family = std::string(json["family"]);
  CFG_ASSERT(device->family.size());
  // series
  CFG_ASSERT(json.contains("series"));
  CFG_ASSERT(json["series"].is_string());
  device->series = std::string(json["series"]);
  CFG_ASSERT(device->series.size());
  // protocol
  CFG_ASSERT(json.contains("protocol"));
  CFG_ASSERT(json["protocol"].is_string());
  device->protocol = std::string(json["protocol"]);
  CFG_ASSERT(device->protocol.size());
  // blwl
  CFG_ASSERT(json.contains("blwl"));
  CFG_ASSERT(json["blwl"].is_string() || json["blwl"].is_null());
  if (json["blwl"].is_string()) {
    device->blwl = std::string(json["blwl"]);
  }
  devices.push_back(device);
}

int main(int argc, const char** argv) {
  CFG_ASSERT(argc == 3);
  int status = 0;
  CFG_POST_MSG("Generate Device Database");
  std::ifstream file(argv[1]);
  CFG_ASSERT(file.is_open() && file.good());
  nlohmann::json configuration = nlohmann::json::parse(file);
  file.close();
  CFG_ASSERT(configuration.is_object());
  std::vector<DEVICE_INFO*> devices;
  for (auto& item : configuration.items()) {
    nlohmann::json key = item.key();
    CFG_ASSERT(key.is_string());
    get_device((std::string)(key), configuration[(std::string)(key)], devices);
  }
  CFG_POST_MSG("  Detected count: %ld", devices.size());
  std::vector<std::string> families;
  std::vector<std::string> seriess;
  std::vector<std::string> protocols;
  std::vector<std::string> blwls;
  for (auto& device : devices) {
    int index = CFG_find_string_in_vector(families, device->family);
    if (index == -1) {
      device->findex = int(families.size());
      families.push_back(device->family);
    } else {
      device->findex = index;
    }
    index = CFG_find_string_in_vector(seriess, device->series);
    if (index == -1) {
      device->sindex = int(seriess.size());
      seriess.push_back(device->series);
    } else {
      device->sindex = index;
    }
    index = CFG_find_string_in_vector(protocols, device->protocol);
    if (index == -1) {
      device->pindex = int(protocols.size());
      protocols.push_back(device->protocol);
    } else {
      device->pindex = index;
    }
    index = CFG_find_string_in_vector(blwls, device->blwl);
    if (index == -1) {
      device->bindex = int(blwls.size());
      blwls.push_back(device->blwl);
    } else {
      device->bindex = index;
    }
  }
  CFGObject_DEV_DDB dev_ddb;
  dev_ddb.write_strs("family", families);
  dev_ddb.write_strs("series", seriess);
  dev_ddb.write_strs("protocol", protocols);
  dev_ddb.write_strs("blwl", blwls);
  for (auto& device : devices) {
    dev_ddb.create_child("device");
    dev_ddb.device.back()->write_str("name", device->name);
    dev_ddb.device.back()->append_u32("data", device->findex);
    dev_ddb.device.back()->append_u32("data", device->sindex);
    dev_ddb.device.back()->append_u32("data", device->pindex);
    dev_ddb.device.back()->append_u32("data", device->bindex);
  }
  while (devices.size()) {
    CFG_MEM_DELETE(devices.back());
    devices.pop_back();
  }
  CFG_POST_MSG("  Writing device database to %s", argv[2]);
  dev_ddb.write(argv[2]);
  return status;
}