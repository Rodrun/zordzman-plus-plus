#include "Level.hpp"
#include "gfx/drawingOperations.hpp"
#include "entity/Player.hpp"
#include "Game.hpp"
#include "level/tiles/Tile.hpp"
#include "format.h"

#include <fstream>
#include <algorithm>

namespace {
int ticks = 0;

// Read all data from a stream into a vector of char
std::vector<char> readAllFromStream(std::istream &stream) {
    std::vector<char> data;
    auto size = stream.tellg();
    data.resize(size);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), size);
    return data;
}
}

Level::Level(std::string const levelname) {
    std::ifstream file(("resources/levels/" + levelname),
                       std::ios::in | std::ios::binary | std::ios::ate);

    // Complain if that Level couldn't be found / opened.
    if (!file.is_open()) {
        fmt::format("[ERROR] Couldn't open level file resources/levels/{}",
                    levelname);
        std::abort();
    }

    // Right now, we're just taking in some basic information about the map.
    auto data = readAllFromStream(file);
    file.close();

    // Width, height, spawn X and spawn Y are the first 4 bytes.
    m_width = data[0];
    m_height = data[1];
    m_spawnx = data[2] * 32;
    m_spawny = data[3] * 32;
    // Resize the vector to match the width and height.
    m_tiles.resize(m_width * m_height);
    // Copy over the level data over to the vector.
    std::copy(data.begin() + 4, data.end(), m_tiles.begin());
}

Level::Level(int width, int height, std::vector<byte> tiles)
    : m_width(width), m_height(height), m_tiles(tiles) {}

void Level::setWidth(byte width) { m_width = width; }

void Level::setHeight(byte height) { m_height = height; }

byte Level::getWidth() const { return m_width; }

byte Level::getHeight() const { return m_height; }

int Level::getSpawnX() const { return m_spawnx; }

int Level::getSpawnY() const { return m_spawny; }

byte Level::tileAt(int x, int y) const { return m_tiles[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) {
    m_tiles[x + y * m_width] = tile;
}

void Level::render() const {
    using namespace drawingOperations;
    auto &window = Game::get().getWindow();

    // Borders for the renders.
    int minX = (int)(0 / 32);
    int maxX = minX + window.getWidth() / 32;

    int minY = (int)(0 / 32);
    int maxY = minY + window.getHeight() / 32;

    // Make sure we don't render anything beyond what's on the screen
    // or part of the level.
    if (minX < 0)
        minX = 0;
    if (maxX > getWidth() - 1)
        maxX = getWidth() - 1;
    if (minY < 0)
        minY = 0;
    if (maxY > getHeight() - 1)
        maxY = getHeight() - 1;

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            drawSpriteFromSheet(Game::get().resources.getSheet("main"),
                                tile::render(tileAt(x, y), ticks), 0, x * 32,
                                y * 32, 32, 32);
        }
    }

    // Render and update the entities.
    for (auto const &e : entities) {
        e->render();
        e->tick();
    }
    ticks++;
}

void Level::add(Entity *e) {
    entities.push_back(std::move(std::unique_ptr<Entity>(e)));
}

Level &Level::operator=(const Level &other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_tiles = other.m_tiles;

    for (auto const &e : other.entities) {
        entities.push_back(std::move(std::unique_ptr<Entity>(e->clone())));
    }

    return *this;
}
