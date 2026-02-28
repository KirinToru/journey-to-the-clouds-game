#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>
#include <vector>

// Structure for text objects from Tiled object layer
struct MapText {
  sf::Vector2f position;
  sf::Vector2f size;
  std::string content;
  std::string name;
};

class Map {
public:
  // Tile size: 32px
  static constexpr float TILE_SIZE = 32.f;

  // Tiled Flip Flags
  static constexpr uint32_t FLIP_H = 0x80000000; // Horizontal flip
  static constexpr uint32_t FLIP_V = 0x40000000; // Vertical flip
  static constexpr uint32_t FLIP_D =
      0x20000000; // Diagonal flip (Anti-diagonal reflection / swap x, y)
  static constexpr uint32_t TILE_MASK =
      0x1FFFFFFF; // Mask to get actual tile ID

  struct TilesetInfo {
    int firstgid;
    int tilewidth;
    int tileheight;
    int tilecount;
    int columns;
    std::string name;
    std::string imageSource;
    sf::Texture texture;
  };

  enum TileType {
    Start = 0,    // Heart
    Finish = 1,   // Star
    Wall = 2,     // Red Block (Solid)
    Platform = 3, // Platform (One-way)
    Spikes = 4    // Hazard
  };

  Map();

  // Loads map from a TMX file (Tiled format)
  bool loadFromFile(const std::string &filename);

  // Getters for map dimensions (in pixels)
  float getWidth() const {
    if (layers.empty() || layers[0].grid.empty())
      return 0.f;
    return layers[0].grid[0].size() * TILE_SIZE;
  }
  float getHeight() const {
    return layers.empty() ? 0.f : layers[0].grid.size() * TILE_SIZE;
  }

  // Returns the player spawn position extracted from the map file
  sf::Vector2f getStartPosition() const { return startPosition; }

  // Renders only the visible portion of the map (view culling)
  void render(sf::RenderWindow &window, sf::Vector2f playerPos = {0, 0},
              bool showHitboxes = false);

  // Checks for collisions between an entity's bounding box and the map walls.
  std::vector<sf::FloatRect> checkCollision(const sf::FloatRect &bounds) const;

  // Checks if the player bounds intersect with the finish tile
  bool checkFinish(const sf::FloatRect &bounds) const;

  // Returns platform tiles that intersect with bounds (one-way platforms)
  std::vector<sf::FloatRect>
  checkPlatformCollision(const sf::FloatRect &bounds) const;

  // Checks if the player bounds intersect with a spike tile
  bool checkSpikeCollision(const sf::FloatRect &bounds) const;

private:
  // Parse TMX XML content
  bool parseTMX(const std::string &content, const std::string &basePath);

  // Parse a single layer's CSV data
  std::vector<std::vector<uint32_t>> parseLayerData(const std::string &csvData,
                                                    int width, int height);

  // Parse object groups (like text hints)
  void parseObjectGroup(const std::string &content);

  // Helpers to prepare rendering data
  void prepareTextObjects();

  struct Layer {
    std::string name;
    std::vector<std::vector<uint32_t>> grid;
  };
  // Map data
  std::vector<Layer> layers;
  std::vector<MapText> textObjects;

  // Cached sf::Text objects for rendering (avoid allocation in render loop)
  std::vector<sf::Text> cachedTexts;

  sf::Vector2f startPosition{100.f, 100.f};
  std::vector<sf::FloatRect> finishAreas;

  // Reusable tile shape (avoid creating new shapes)
  sf::RectangleShape tileShape;

  // Data storages
  std::vector<TilesetInfo> tilesets;

  // Cached objects
  sf::Font font;
  bool fontLoaded = false;

  // Helpers
  const TilesetInfo *getTilesetForId(int globalId) const;
};
