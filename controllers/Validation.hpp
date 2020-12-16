#pragma once

#include <drogon/drogon.h>
#include <iostream>
#include <string>
#include "sha1.hpp"
#include "SystemDao.hpp"

bool validateRequest(const drogon::HttpRequestPtr &req);

