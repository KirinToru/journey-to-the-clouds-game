#pragma once
#include <SFML/Graphics.hpp>
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

  Map();

  // Loads map from a TMX file (Tiled format)
  bool loadFromFile(const std::string &filename);

  // Getters for map dimensions (in pixels)
  float getWidth() const {
    return mainGrid.empty() ? 0.f : mainGrid[0].size() * TILE_SIZE;
  }
  float getHeight() const { return mainGrid.size() * TILE_SIZE; }

  // Returns the player spawn position extracted from the map file
  sf::Vector2f getStartPosition() const { return startPosition; }

  // Renders the map tiles and text to the window
  void render(sf::RenderWindow &window);

  // Checks for collisions between an entity's bounding box and the map walls.
  // Returns a vector of bounding boxes of the tiles that intersect.
  std::vector<sf::FloatRect> checkCollision(const sf::FloatRect &bounds) const;

  // Checks if the player bounds intersect with the finish tile
  bool checkFinish(const sf::FloatRect &bounds) const;

private:
  // Parse TMX XML content
  bool parseTMX(const std::string &content);

  // Parse a single layer's CSV data
  std::vector<std::vector<int>> parseLayerData(const std::string &csvData,
                                               int width, int height);

  // Parse object group for text objects
  void parseObjectGroup(const std::string &content);

  // Main grid for collision detection (from "main" layer)
  // IDs: 0=empty, 1=spawn, 2=finish, 3=wall, 4=text marker
  std::vector<std::vector<int>> mainGrid;

  // Texture grid for rendering (from "textures" layer)
  // IDs: 0=empty, 5=spawn, 6=finish, 7=wall
  std::vector<std::vector<int>> textureGrid;

  // Text objects from object layer
  std::vector<MapText> textObjects;

  sf::Vector2f startPosition{100.f, 100.f}; // Default if not found
  std::vector<sf::FloatRect> finishAreas;   // Multiple finish zones allowed

  // Helper to setup shapes for rendering
  sf::RectangleShape tileShape;

  // Textures for tiles
  sf::Texture finishTexture;
  sf::Texture spawnTexture;
  sf::Texture wallTexture;

  // Font for text rendering
  sf::Font font;
  bool fontLoaded;
};
