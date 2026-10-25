//sim callbacks
#include "../src/simulation.hpp"
#include "controls.hpp"

static bool  button_left = false, button_middle = false, button_right = false;
static double lastx = 0, lasty = 0;

void glfw_error_callback(int code, const char* desc){
    printf("error bruh");
}

static void mouse_button_callback(GLFWwindow* window, int button, int act, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)   button_left   = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_left);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) button_middle = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_middle);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)  button_right  = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_right);

    glfwGetCursorPos(window, &lastx, &lasty);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    scn = getScene(); cam = getCam();
    if (!m) return;
    if (!button_left && !button_right && !button_middle) {
        lastx = xpos;
        lasty = ypos;
        return;
    }

    int fbw = 0, fbh = 1;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    if (fbh <= 0) fbh = 1;

    double dx = (xpos - lastx) / (double)fbh;
    double dy = (ypos - lasty) / (double)fbh;
    lastx = xpos;
    lasty = ypos;

    if (button_left) {
        mjv_moveCamera(m, mjMOUSE_ROTATE_H, dx, dy, &scn, &cam);
    } else if (button_right) {
        mjv_moveCamera(m, mjMOUSE_MOVE_H, dx, dy, &scn, &cam);
    } else if (button_middle) {
        mjv_moveCamera(m, mjMOUSE_ZOOM, dx, dy, &scn, &cam);
    }
}

static void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    scn = getScene(); cam = getCam();
    if (!m) return;
    mjv_moveCamera(m, mjMOUSE_ZOOM, 0, 0.1 * yoffset, &scn, &cam); //-.05 tadinya
}

//key yg sdg dipencet
int currentKey = 0;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
    if (act == GLFW_RELEASE){
        currentKey = 0;
    }
    if (act != GLFW_PRESS) return;
    //currentKey = 0;
    currentKey = key;

    for (auto& p : PoseList) {
        if (key == p->getKey()) playPose(*p);
    };
}

void keyHeld_update(mjData* d){
    switch (currentKey){
        //torso yaw
        case GLFW_KEY_N:
            moveJoint(0, -1); break;
        case GLFW_KEY_M: 
            moveJoint(0, 1); break;
        //shoulder pitch
        case GLFW_KEY_Z: 
            moveJoint(1, -1); break;
        case GLFW_KEY_X: 
            moveJoint(1, 1); break;
        //upper arm pitch
        case GLFW_KEY_A: 
            moveJoint(2, -1); break;
        case GLFW_KEY_S: 
            moveJoint(2, 1); break;
        //forearm pitch
        case GLFW_KEY_Q: 
            moveJoint(3, -1); break;
        case GLFW_KEY_W: 
            moveJoint(3, 1); break;
        //hand yaw
        case GLFW_KEY_J: 
            moveJoint(4, -1); break;
        case GLFW_KEY_K: 
            moveJoint(4, 1); break;
    }
}

