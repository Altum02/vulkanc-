#include "app.hpp"

#include <memory>

int main(){
    std::shared_ptr<mv::App> app = std::make_shared<mv::App>();
    app->run();
}