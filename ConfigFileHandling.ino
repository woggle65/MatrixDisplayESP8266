bool loadSysConfig()
{
  File configFile = SPIFFS.open("/" + configFilename, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, buf.get());

  if (error) {
    Serial.println("loadSystemConfig JSON DeserializationError");
    return false;
  }
  JsonObject json = doc.as<JsonObject>();

  serializeJson(doc, Serial); Serial.println();
  
  ((json["ip"]).as<String>()).toCharArray(ip, IPSIZE);
  ((json["netmask"]).as<String>()).toCharArray(netmask, IPSIZE);
  ((json["gw"]).as<String>()).toCharArray(gw, IPSIZE);

  ((json["refreshSeconds"]).as<String>()).toCharArray(refreshSeconds, 10);
  ((json["scrollPause"]).as<String>()).toCharArray(scrollPause, 10);
  ((json["scrollSpeed"]).as<String>()).toCharArray(scrollSpeed, 10);
  ((json["url"]).as<String>()).toCharArray(url, 255);
  ((json["ccuip"]).as<String>()).toCharArray(ccuip, IPSIZE);
  ((json["sysvar"]).as<String>()).toCharArray(sysvar, 255);

  intensity = json["intensity"];

  if (String(ccuip) == "null")
  {
    ccuip[0] = 0;
  }

  if (String(sysvar) == "null")
  {
    sysvar[0] = 0;
  }

  Serial.println("IP: " + String(ip));
  Serial.println("NetMask: " + String(netmask));
  Serial.println("Gateway: " + String(gw));

  Serial.println("Refresh: " + String(refreshSeconds));
  Serial.println("ScrollPause: " + String(scrollPause));
  Serial.println("ScrollSpeed: " + String(scrollSpeed));
  Serial.println("URL: " + String(url));
  Serial.println("CCU IP: " + String(ccuip));
  Serial.println("SysVar: " + String(sysvar));

  Serial.print("Loaded intensity: "); Serial.println(String(intensity));

  return true;
}

bool saveSysConfig()
{
  StaticJsonDocument<1024> doc;
  JsonObject json = doc.to<JsonObject>();

  json["ip"] = ip;
  json["netmask"] = netmask;
  json["gw"] = gw;
  json["refreshSeconds"] = refreshSeconds;
  json["scrollPause"] = scrollPause;
  json["scrollSpeed"] = scrollSpeed;
  json["url"] = url;
  json["ccuip"] = ccuip;
  json["sysvar"] = sysvar;
  json["intensity"] = intensity;

  File configFile = SPIFFS.open("/" + configFilename, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);

  serializeJson(doc, Serial); Serial.println();
  
  return true;
}
