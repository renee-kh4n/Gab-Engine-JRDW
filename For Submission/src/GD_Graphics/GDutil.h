#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>


inline std::string LoadTxtFile(std::string path) {
    std::fstream fsrc(path);
    std::stringstream fbuff;
    fbuff << fsrc.rdbuf();

    return fbuff.str();
}

inline float getSqrMag(glm::vec3 vec) {
    return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
}

inline glm::vec3 EulerToVec(glm::vec3 basis, glm::vec2 rot) {
    basis = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), basis), glm::radians(rot.x), glm::vec3(-1, 0, 0)) * glm::vec4(basis, 0));
    basis = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), basis), glm::radians(rot.y), glm::vec3(0, 1, 0)) * glm::vec4(basis, 0));

    return -basis;
}
inline glm::vec3 RotVec(glm::vec3 basis, glm::vec3 axis, float angle) {
    basis = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), basis), glm::radians(angle), axis) * glm::vec4(basis, 0));

    return -basis;
}