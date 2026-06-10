//
// Created by alexk on 04.06.2026.
//

#ifndef CITYBUILDER_BT_NODE_FACTORY_H
#define CITYBUILDER_BT_NODE_FACTORY_H
#include <memory>

#include "bt_action.h"
#include "bt_sequence.h"
#include "bt_selector.h"

namespace core::ai::behaviour_tree
{
    template<typename Taction>
    auto MakeAction(Taction callback_) {
        return std::make_unique<ActionNode<Taction>>(callback_);
    };

    inline auto MakeSequence(){
        return std::make_unique<SequenceNode>();
    }

    inline auto MakeSelector()
    {
        return std::make_unique<SelectorNode>();
    }

}
#endif //CITYBUILDER_BT_NODE_FACTORY_H
