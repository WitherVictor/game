#include "view/view.hpp"
#include "model/model.hpp"

int main() {
    model model_;
    view view_{model_};

    view_.run();
}