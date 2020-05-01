#include <litequarks/LQAppController.hpp>
#include <litequarks/LQAppModel.hpp>
#include <litequarks/LQRawData.hpp>

#include "../../app/components.hpp"

std::queue<LQEvent*>
LQAppController::s_eventQueue;

std::unordered_map<std::pair<std::type_index, const void*>,
    std::function<void(LQEvent*)>>
LQAppController::s_eventDispatcher;

std::unordered_map<std::string,
    std::vector<std::pair<std::function<void(void*, void*)>, void*>>>
LQAppController::s_modelObservers;

std::mutex
LQAppController::s_mutex;

std::queue<char*>
LQAppController::s_queries;

std::queue<char*>
LQAppController::s_responses;

ClientGateway*
LQAppController::s_gateway = nullptr;

std::thread*
LQAppController::s_gatewayThread = nullptr;

LQWindow*
LQAppController::s_window = nullptr;

LQViewable*
LQAppController::s_hover_focus = nullptr;

LQViewable*
LQAppController::s_focus = nullptr;

float
LQAppController::prevAbsX = 0;

float
LQAppController::prevAbsY = 0;

float
LQAppController::prevRelX = 0;

float
LQAppController::prevRelY = 0;

void LQAppController::init() {
    lqOn<LQDataQueryEvent>(dataQueryCallback);
    lqOn<LQModelUpdateEvent>(modelUpdateCallback);
    lqOn<loginEvent>(loginCallback); // Dans la class bouton login
    lqOn<registerEvent>(registerCallback); // Dans la class bouton register
    lqOn<upProjectEvent>(upProjectCallback); // Dans la class bouton projet
    lqOn<dlProjectEvent>(dlProjectCallback); // Dans la class bouton projet
    s_gateway = new ClientGateway(s_queries, s_responses);
    s_gatewayThread = new std::thread(std::ref(*s_gateway));
}

void LQAppController::finalize() {
    // TODO : undo lqON (here)
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[1];
    query[0] = 0;
    s_queries.push(query);

    s_gatewayThread->join();
    delete s_gatewayThread;
    delete s_gateway;
}

void LQAppController::pushEvent(LQEvent* event) {
    s_eventQueue.push(event);
}

void LQAppController::pushQuery(char* query) {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_queries.push(query);
}

void LQAppController::addDispatcher(
    std::type_index&& eventType, void* target,
    std::function<void(LQEvent*)> dispatcher)
{
    s_eventDispatcher[std::make_pair(eventType, target)] = dispatcher;
}

void LQAppController::addObserver(
    const std::string& model, void* observer, void (*callback)(void*, void*))
{
    s_modelObservers[model].push_back(std::make_pair(callback, observer));
}

void LQAppController::pollEvents() {
    while (!s_eventQueue.empty()) {
        auto event = s_eventQueue.front();
        auto dispatch =
            s_eventDispatcher[std::make_pair(event->type, event->target)];
        dispatch(event);
        delete event;
        s_eventQueue.pop();
    }
}

void LQAppController::pollResponses() {
    std::lock_guard<std::mutex> lock(s_mutex);
    while (!s_responses.empty()) {
        LQRawData data(s_responses.front());
        std::string type = data.parse<char*>();
        if(type == "dataReceive") {
            LQAppController::pushEvent(new LQDataReceivedEvent(data));
        }
        if (type == "login") {
            int rep = data.parse<int8_t>();
            if (rep == 0) {
                // window->SignInView->document->errorField
                auto* errorSignUp = static_cast<LQViewable*>(
                    s_window->firstChild()->firstChild()->lastChild()->prevSibling());
                auto* errorSignIn = static_cast<LQViewable*>(errorSignUp->prevSibling());
                errorSignUp->hide();
                errorSignIn->unhide();
            }
            else {
                auto* signInView = static_cast<SignInView*>(s_window->firstChild());
                s_window->removeFirstChild();
                s_window->appendChild(signInView->navbar);
                s_window->appendChild(signInView->viewport);
                LQAppController::resetMousePosition();
                delete signInView;
            }
        }
        if (type == "register") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                // window->SignInView->document->errorField
                auto* errorSignUp = static_cast<LQViewable*>(
                    s_window->firstChild()->firstChild()->lastChild()->prevSibling());
                auto* errorSignIn = static_cast<LQViewable*>(errorSignUp->prevSibling());
                errorSignIn->hide();
                errorSignUp->unhide();
            }
            else {
                // window->SignInView->document->hidden->icon->login;
                auto* login = static_cast<LQTextArea*>(
                    s_window->firstChild()->firstChild()->firstChild()->nextSibling()->nextSibling());
                auto * password = static_cast<LQTextArea*>(login->nextSibling());
                s_eventQueue.push(new loginEvent(login->getContent(), password->getContent()));
            }
        }
        if (type == "upProject") {
            int rep = data.parse<int8_t>();
            auto* valid = static_cast<LQViewable*>(
                s_window->lastChild()->firstChild()->lastChild());
            auto* error = static_cast<LQViewable*>(valid->prevSibling());

            if (rep == 1) {  // name not taken
                // window->DropProjectView->document->valid
                error->hide();
                valid->unhide();
            }
            else {
                valid->hide();
                error->unhide();
            }
        }
        if(type == "projectUploaded") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                //Erreur upload projet
            }
        }
        if(type == "projectDownloaded") {
            int rep = data.parse<int8_t>();
            if(rep == 0) {
                //Erreur download projet
            }
            else {
                //Projet téléchargé
            }
        }
        s_responses.pop();
    }
}

void LQAppController::setWindow(LQWindow* window) {
    s_window = window;
    if (!s_hover_focus) {
        s_hover_focus = s_window;
        s_focus = s_window;
    }
}

void LQAppController::removeFocus(LQViewable* viewable) {
    if (viewable == s_hover_focus) {
        s_hover_focus = s_window;
        prevRelX = prevAbsX;
        prevRelY = prevAbsY;
    }
    if (viewable == s_focus) {
        s_focus = s_window;
    }
}

void LQAppController::resetMousePosition() {
    prevAbsX = prevAbsY = prevRelX = prevRelY = 0;
    s_hover_focus = s_window;
    if (hasCallback<LQFocusLoseEvent>(s_focus)) {
        s_eventQueue.push(new LQFocusLoseEvent(s_focus));
    }
    s_eventQueue.push(new LQFocusGainEvent(s_window));
    s_focus = s_window;
}

void LQAppController::updateMousePosition() {
    double mx, my;
    glfwGetCursorPos(s_window->getGLFWwindow(), &mx, &my);
    cursor_position_callback(s_window->getGLFWwindow(), mx, my);
}

void LQAppController::recalcMousePosition(float xoffset, float yoffset) {
    prevRelX += xoffset;
    prevRelY += yoffset;
    updateMousePosition();
}

void LQAppController::cursor_position_callback(GLFWwindow* window, double mx, double my) {
    if (my < 50 && prevAbsY >= 50) {  // mouse over navbar
        s_hover_focus = static_cast<LQViewable*>(s_window->firstChild());
        prevAbsX = prevRelX = mx;
        prevAbsY = prevRelY = my;
        return;
    }
    float deltaX = mx - prevAbsX;
    float deltaY = my - prevAbsY;
    prevRelX += deltaX;
    prevRelY += deltaY;

    LQViewable* current = s_hover_focus;
    bool outCurrent = prevRelX < 0 || prevRelX > current->widthF() ||
                      prevRelY < 0 || prevRelY > current->heightF();

    while (outCurrent && current != s_window) {
        prevRelX += current->xF();
        prevRelY += current->yF();
        current = static_cast<LQViewable*>(current->parent());
        outCurrent = prevRelX < 0 || prevRelX > current->widthF() ||
                     prevRelY < 0 || prevRelY > current->heightF();
    }

    s_hover_focus = current;
    current = static_cast<LQViewable*>(current->firstChild());
    // On recherche un fils correspondant à notre position
    while (current) {
        outCurrent =
            prevRelX < current->xF() || prevRelY < current->yF() ||
            prevRelX > current->xF() + current->widthF() ||
            prevRelY > current->yF() + current->heightF();
        if (outCurrent) {
            current = static_cast<LQViewable*>(current->nextSibling());
        }
        else {
            s_hover_focus = current;
            prevRelX -= current->xF();
            prevRelY -= current->yF();
            current = static_cast<LQViewable*>(current->firstChild());
        }
    }

    prevAbsX = mx;
    prevAbsY = my;
}

void LQAppController::mouse_button_callback(
    GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto eligible = getEligible<LQFocusGainEvent>();
        if (s_focus != eligible) {
            if (hasCallback<LQFocusLoseEvent>(s_focus)) {
                s_eventQueue.push(new LQFocusLoseEvent(s_focus));
            }
            s_eventQueue.push(new LQFocusGainEvent(eligible));
            s_focus = eligible;
        }
        auto* eligibleToClick = getEligible<LQClickEvent>();
        s_eventQueue.push(new LQClickEvent(eligibleToClick, prevRelX, prevRelY));
    }
}

void LQAppController::key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (hasCallback<LQKeyEvent>(s_focus)) {
        s_eventQueue.push(new LQKeyEvent(s_focus, key, action, mods));
    }
}

void LQAppController::character_callback(GLFWwindow* window, unsigned int codepoint) {
    if (hasCallback<LQCharEvent>(s_focus)) {
        s_eventQueue.push(new LQCharEvent(s_focus, codepoint));
    }
}

void LQAppController::scroll_callback(
    GLFWwindow* window, double xoffset, double yoffset)
{
    auto eligible = getEligible<LQScrollEvent>();
    s_eventQueue.push(new LQScrollEvent(eligible, xoffset, yoffset));
}

void LQAppController::drop_callback(
    GLFWwindow* window, int count, const char** _paths)
{
    std::vector<std::string> paths;
    for (int i = 0; i < count; ++i) {
        paths.push_back(std::string(_paths[i]));
    }
    auto eligible = getEligible<LQDropEvent>();
    s_eventQueue.push(new LQDropEvent(eligible, std::move(paths)));
}

void LQAppController::dataQueryCallback(LQDataQueryEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.query.length() + 2];
    query[0] = 1;
    strcpy(&query[1], event.query.c_str());
    s_queries.push(query);
}

void LQAppController::modelUpdateCallback(LQModelUpdateEvent& event) {
    for (auto& info : event.infos) {
        auto items = LQAppModel::get(info.first);
        auto& observers = s_modelObservers[info.first];
        auto it = items.cbegin() + info.second;

        for (auto& pair : observers) {
            for (auto item = it; item != items.cend(); ++item) {
                (pair.first)(pair.second, *item);
            }
        }
    }
}

void LQAppController::loginCallback(loginEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 2;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::registerCallback(registerEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 3;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::upProjectCallback(upProjectEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[event.infos.length() + 2];
    query[0] = 4;
    strcpy(&query[1], event.infos.c_str());
    s_queries.push(query);
}

void LQAppController::dlProjectCallback(dlProjectEvent& event) {
    std::lock_guard<std::mutex> lock(s_mutex);
    char* query = new char[5];
    query[0] = 5;
    memcpy(&query[1], &event.idProject, 4);
    s_queries.push(query);
}
