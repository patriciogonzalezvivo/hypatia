//
//  TleException.h
//
//  Created by Patricio González Vivo on 5/31/18.
//

#pragma once

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error {
public:
    Exception(const char* message) : runtime_error(message) { }
};
