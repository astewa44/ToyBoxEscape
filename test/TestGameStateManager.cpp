#include <catch.hpp>
#include "GameStateManager.hpp"

#include <vector>

struct Empty { };

struct AllMethods
{
    int* update_count;
    int* draw_count;
    bool haltsUpdate() { return false; }
    bool haltsDraw() { return false; }
    void update() { ++(*update_count); }
    void draw() { ++(*draw_count); }
};

struct AllNonmembers
{
    int* update_count;
    int* draw_count;
};

bool haltsUpdate(AllNonmembers&) { return false; }
bool haltsDraw(AllNonmembers&) { return false; }
void update(AllNonmembers& allnm) { ++(*allnm.update_count); }
void draw(AllNonmembers& allnm) { ++(*allnm.draw_count); }

struct Updater
{
    int* update_count;
    bool haltsUpdate() { return false; }
    void update() { ++(*update_count); }
};

struct Drawer
{
    int* draw_count;
    bool haltsDraw() { return false; }
    void draw() { ++(*draw_count); }
};

struct Halter
{
    bool haltsUpdate() { return true; }
    bool haltsDraw() { return true; }
};

struct UpdaterOrdered
{
    int id;
    std::vector<int>* vec;
    void update() { vec->push_back(id); }
};

struct DrawerOrdered
{
    int id;
    std::vector<int>* vec;
    void draw() { vec->push_back(id); }
};

TEST_CASE("GameStateManager detects methods properly", "[GameStateManager]")
{

    int update_count = 0;
    int draw_count = 0;

    auto empty = Empty{};
    auto allm = AllMethods{&update_count, &draw_count};
    auto allnm = AllNonmembers{&update_count, &draw_count};

    SECTION("empty types work")
    {
        GameStateManager GSM;
        GSM.pushState(empty);
        GSM.pushState(empty);
        GSM.pushState(empty);
        update_count = 0;
        draw_count = 0;
        GSM.update();
        GSM.draw();
        REQUIRE(update_count == 0);
        REQUIRE(draw_count == 0);
    }

    SECTION("methods work")
    {
        GameStateManager GSM;
        GSM.pushState(allm);
        GSM.pushState(allm);
        GSM.pushState(allm);
        update_count = 0;
        draw_count = 0;
        GSM.update();
        GSM.draw();
        REQUIRE(update_count == 3);
        REQUIRE(draw_count == 3);
    }

    SECTION("nonmember functions work")
    {
        GameStateManager GSM;
        GSM.pushState(allnm);
        GSM.pushState(allnm);
        GSM.pushState(allnm);
        update_count = 0;
        draw_count = 0;
        GSM.update();
        GSM.draw();
        REQUIRE(update_count == 3);
        REQUIRE(draw_count == 3);
    }
}

TEST_CASE("GameStateManager pushes and pops properly", "[GameStateManager]")
{

    SECTION("empty GSM causes no updates")
    {
        GameStateManager GSM;
        int update_count = 0;
        Updater updater{&update_count};

        GSM.update();
        REQUIRE(update_count == 0);
    }

    SECTION("one push causes one update")
    {
        GameStateManager GSM;
        int update_count = 0;
        Updater updater{&update_count};

        GSM.pushState(updater);
        GSM.update();
        REQUIRE(update_count == 1);
    }

    SECTION("pop causes one less update")
    {
        GameStateManager GSM;
        int update_count = 0;
        Updater updater{&update_count};

        GSM.pushState(updater);
        GSM.update();
        REQUIRE(update_count == 1);
        GSM.popState();
        update_count = 0;
        GSM.update();
        REQUIRE(update_count == 0);
    }
}

TEST_CASE("GameStateManager updates properly", "[GameStateManager]")
{

    int update_count = 0;

    Updater updater{&update_count};
    Halter halter;

    SECTION("empty GSM causes no updates")
    {
        GameStateManager GSM;
        update_count = 0;
        GSM.update();
        REQUIRE(update_count == 0);
    }

    SECTION("three halters and three updaters cause three updates")
    {
        GameStateManager GSM;
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(updater);
        GSM.pushState(updater);
        GSM.pushState(updater);

        update_count = 0;
        GSM.update();
        REQUIRE(update_count == 3);
    }

    SECTION("updaters that are below halters do not get updated")
    {
        GameStateManager GSM;
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(updater);
        GSM.pushState(updater);
        GSM.pushState(halter);
        GSM.pushState(updater);

        update_count = 0;
        GSM.update();
        REQUIRE(update_count == 1);
    }

    SECTION("updates happen in order of last-in, first-updated")
    {
        GameStateManager GSM;
        std::vector<int> vec;

        GSM.pushState(UpdaterOrdered{0, &vec});
        GSM.pushState(UpdaterOrdered{1, &vec});
        GSM.pushState(UpdaterOrdered{2, &vec});
        GSM.pushState(UpdaterOrdered{3, &vec});
        GSM.pushState(UpdaterOrdered{4, &vec});
        GSM.pushState(UpdaterOrdered{5, &vec});

        vec.clear();
        GSM.update();
        REQUIRE(vec.size() == 6u);
        REQUIRE(vec[0] == 5);
        REQUIRE(vec[1] == 4);
        REQUIRE(vec[2] == 3);
        REQUIRE(vec[3] == 2);
        REQUIRE(vec[4] == 1);
        REQUIRE(vec[5] == 0);
    }
}

TEST_CASE("GameStateManager draws properly", "[GameStateManager]")
{

    int draw_count = 0;

    Drawer drawer{&draw_count};
    Halter halter;

    SECTION("empty GSM causes no draws")
    {
        GameStateManager GSM;
        draw_count = 0;
        GSM.draw();
        REQUIRE(draw_count == 0);
    }

    SECTION("three halters and three drawers cause three draws")
    {
        GameStateManager GSM;
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(drawer);
        GSM.pushState(drawer);
        GSM.pushState(drawer);

        draw_count = 0;
        GSM.draw();
        REQUIRE(draw_count == 3);
    }

    SECTION("drawers that are below halters do not get drawd")
    {
        GameStateManager GSM;
        GSM.pushState(halter);
        GSM.pushState(halter);
        GSM.pushState(drawer);
        GSM.pushState(drawer);
        GSM.pushState(halter);
        GSM.pushState(drawer);

        draw_count = 0;
        GSM.draw();
        REQUIRE(draw_count == 1);
    }

    SECTION("draws happen in order of last-in, first-drawn")
    {
        GameStateManager GSM;
        std::vector<int> vec;

        GSM.pushState(DrawerOrdered{0, &vec});
        GSM.pushState(DrawerOrdered{1, &vec});
        GSM.pushState(DrawerOrdered{2, &vec});
        GSM.pushState(DrawerOrdered{3, &vec});
        GSM.pushState(DrawerOrdered{4, &vec});
        GSM.pushState(DrawerOrdered{5, &vec});

        vec.clear();
        GSM.draw();
        REQUIRE(vec.size() == 6u);
        REQUIRE(vec[0] == 0);
        REQUIRE(vec[1] == 1);
        REQUIRE(vec[2] == 2);
        REQUIRE(vec[3] == 3);
        REQUIRE(vec[4] == 4);
        REQUIRE(vec[5] == 5);
    }
}

TEST_CASE("GameStateManager is a State", "[GameStateManager]")
{
    SECTION("GameStateManager can be stored in a GameStateManager")
    {
        GameStateManager GSM;
        int update_count = 0;
        int draw_count = 0;
        Updater updater{&update_count};
        Drawer drawer{&draw_count};
        GameStateManager L2GSM;
        L2GSM.pushState(updater);
        L2GSM.pushState(drawer);
        GSM.pushState(L2GSM);
        GSM.update();
        GSM.draw();
        REQUIRE(update_count == 1);
        REQUIRE(draw_count == 1);
    }
}
