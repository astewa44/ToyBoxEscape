#include "GameStateManager.hpp"

#include "Ranges.hpp"

namespace {

#define GSM_CAT(A, B) A ## B
#define GSM_THUNK(NAME) \
    template <typename... Ts> \
    auto GSM_CAT(call_,NAME)(Ts&&... ts) \
		    -> decltype(NAME(std::forward<Ts>(ts)...)) \
	{ return NAME(std::forward<Ts>(ts)...); }

GSM_THUNK(haltsUpdate)
GSM_THUNK(haltsDraw)
GSM_THUNK(update)
GSM_THUNK(draw)

#undef GSM_THUNK
#undef GSM_CAT

} // static

namespace _detail_GameStateManager {

void GameStateManager::push(StateErasure state)
{
    states.emplace_back(state);
}

void GameStateManager::pop()
{
    states.pop_back();
}

bool GameStateManager::empty()
{
	return states.empty();
}

int GameStateManager::size()
{
    return states.size();
}

void GameStateManager::update()
{
    for (auto& state : reverse(states))
    {
        bool halts = call_haltsUpdate(state);
        call_update(state);
        if (halts)
        {
            return;
        }
    }
}

void GameStateManager::draw()
{
    auto findStart = [&]
    {
        for (auto riter = rbegin(states), eriter = rend(states); riter != eriter; ++riter)
        {
			auto iter = next(riter).base();
            auto halts = call_haltsDraw(*iter);
            if (halts)
            {
                return iter;
            }
        }
        return begin(states);
    };

    auto start = findStart();
    for (auto& state : make_iterator_range(start, end(states)))
    {
        call_draw(state);
    }
}

} // namespace _detail_GameStateManager
