#ifndef TOYBOXESCAPE_GAMESTATEMANAGER_HPP
#define TOYBOXESCAPE_GAMESTATEMANAGER_HPP

#include "Ranges.hpp"

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/mpl/vector.hpp>

#include <utility>
#include <vector>
#include <memory>

// Reflection stuff

namespace _detail_GameStateManager {

// Defaults

#define GSM_CAT(A, B) A ## B
#define GSM_DECL_DEFAULT(NAME, PARAM) \
    template<typename T, typename... Ts, typename=decltype(std::declval<T>().NAME(std::declval<Ts&&>()...))> \
    auto GSM_CAT(impl_,NAME)(int, T& t, Ts&&... ts) { return t.NAME(std::forward<Ts>(ts)...); } \
    template<typename T, typename... Ts> \
    auto GSM_CAT(impl_,NAME)(long, T&, Ts&&...); \
    template<typename T, typename... Ts> \
    auto NAME(T& t, Ts&&... ts) { return GSM_CAT(impl_,NAME)(0, t, std::forward<Ts>(ts)...); } \
    template<typename T, typename... Ts> \
    auto GSM_CAT(impl_,NAME)(long, T& PARAM, Ts&&... ts)

GSM_DECL_DEFAULT(haltsUpdate,) { return false; }
GSM_DECL_DEFAULT(haltsDraw,) { return false; }
GSM_DECL_DEFAULT(update,) { }
GSM_DECL_DEFAULT(draw,) { }

#undef GSM_DECL_DEFAULT
#undef GSM_CAT

} // namespace _detail_GameStateManager

BOOST_TYPE_ERASURE_FREE((_detail_GameStateManager)(has_haltsUpdate), haltsUpdate,1)
BOOST_TYPE_ERASURE_FREE((_detail_GameStateManager)(has_haltsDraw), haltsDraw,1)
BOOST_TYPE_ERASURE_FREE((_detail_GameStateManager)(has_update), update,1)
BOOST_TYPE_ERASURE_FREE((_detail_GameStateManager)(has_draw), draw,1)

namespace _detail_GameStateManager {

namespace te = boost::type_erasure;

// Concepts

using State = boost::mpl::vector<
        te::copy_constructible<>,
        has_haltsUpdate<bool(te::_self&)>,
        has_haltsDraw<bool(te::_self&)>,
        has_update<void(te::_self&)>,
        has_draw<void(te::_self&)>,
        te::relaxed
>;
// Erasure Type

using StateErasure = te::any<State>;

// State manager

/**
 * Manages a stack of game states
 *
 * Manages and provides access to a stack of game states.
 * A game state can be anything.
 *
 * Absolutely not thread-safe.
 */
class GameStateManager final
{
    std::vector<StateErasure> states;
public:

    /**
     * Pushes a state onto the stack
     *
     * Pushes a state onto the top of the stack.
     * This state will be updated first and drawn last.
     *
     * This function must not be called while update() or draw() is running.
     *
     * @param state Game state
     */
    void push(StateErasure state);

    /**
     * Pops a state off the stack
     *
     * Pops the current top state off the stack.
     *
     * This function must not be called while update() or draw() is running.
     */
    void pop();

    /**
     * Update states
     *
     * Updates all states on the stack, from top to bottom.
     * This is done by calling `state.update()` or `update(state)`,
     * if available.
     *
     * A state may provide `state.haltsUpdate()` or `haltsUpdate(state)`.
     * If that function returns `true`, this state will prevent states
     * below it from being updated.
     */
    void update();

    /**
     * Draw states
     *
     * Draws all states on the stack, from bottom to top.
     * This is done by calling `state.draw()` or `draw(state)`,
     * if available.
     *
     * A state may provide `state.haltsDraw()` or `haltsDraw(state)`.
     * If that function returns `true`, this state will prevent states
     * below it from being drawn.
     */
    void draw();

	bool empty();

    int size();
};

} // namespace _detail_GameStateManager

using _detail_GameStateManager::GameStateManager;
using _detail_GameStateManager::update;
using _detail_GameStateManager::draw;
using _detail_GameStateManager::StateErasure;

#endif // TOYBOXESCAPE_GAMESTATEMANAGER_HPP
