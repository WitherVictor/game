#include "view/view.hpp"
#include "model/model.hpp"

#include "item/food.hpp"
#include <memory>

int main() {
    view view_{model::instance()};

    model::instance().inventory.add_item(std::make_shared<protein_bar>(3));

    view_.run();
}