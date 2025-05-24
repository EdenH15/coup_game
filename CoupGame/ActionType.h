//
// Created by eden on 5/11/25.
//

#ifndef ACTION_TYPE_H
#define ACTION_TYPE_H

namespace CoupG {
    enum class ActionType {
        None,
        Gather,
        Tax,
        Bribe,
        Arrest,
        Sanction,
        Coup,
        Invest,
        Spy,
        UndoTax
    };


}

#endif //ACTION_TYPE_H
