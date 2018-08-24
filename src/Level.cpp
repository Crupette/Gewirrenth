#include "Level.h"
#include "ErrorHandler.h"
#include "Base.h"
#include "Game.h"
#include "Player.h"
#include "EntityRenderer.h"
#include "TileRegistry.h"
#include "RoguelikeMath.h"

#include <fstream>
#include <sstream>
#include <ctime>

std::mt19937 Level::m_randomEngine(time(0)+1024);

Level::Level() : m_cleared(false), m_persistWidth(0), m_persistHeight(0), m_id(0), m_nextId(1), m_level(1), m_currentLevel(0), m_boss(false), m_numEnemies(0)
{
}


Level::~Level()
{
}

void Level::init()
{
	TileRenderer::init();
	EntityRenderer::init();

	generateTitleScreen();
}

void Level::generateLevel(unsigned int newId)
{
	m_level++;

	int difficulty = (int)floor(m_level / 5.f);
	int numDoors = 0;

	Player* player = Base::getGame()->getPlayer();

	cleanData();

	m_cleared = false;

	if (m_level % 5 == 0) {
		player->setPosition(glm::ivec2(1));
		generateBossLevel();
		renderTiles();
		return;
	}
	
	std::uniform_int_distribution<int> sizeDist(10, 100);
	std::uniform_real_distribution<float> enemyMult(0.5f, 1.f + (float)(difficulty / 10.f));

	int oldWidth = m_width;
	int oldHeight = m_height;
	
	m_width = m_persistWidth ? m_width : sizeDist(m_randomEngine);
	m_height = m_persistHeight ? m_height : sizeDist(m_randomEngine);

	glm::ivec2 enterInverse(
		m_enterPos.x == 0 ? m_width-1 : (m_enterPos.x == oldWidth-1 ? 0 : m_enterPos.x),
		m_enterPos.y == 0 ? m_height - 1 : (m_enterPos.y == oldHeight - 1 ? 0 : m_enterPos.y)
	);

	unsigned int enemyMin = (int)((m_width + m_height / 2.f) / 20.f) * (difficulty + 1);
	unsigned int enemyMax = (int)((m_width + m_height / 2.f) / 20.f) * (difficulty + 1) * (int)(enemyMult(m_randomEngine));

	std::uniform_int_distribution<int> enemyDist(enemyMin > enemyMax ? enemyMax : enemyMin, enemyMin > enemyMax ? enemyMin : enemyMax);
	std::uniform_int_distribution<int> doorDist(1, (m_width + m_height)/3);
	std::uniform_int_distribution<int> rubbleDist(1, (int)((m_width + m_height) / 2.f) - difficulty);

	size_t numEnemies = enemyDist(m_randomEngine);

	for (size_t y = 0; y < m_height; y++) {
		for (size_t x = 0; x < m_width; x++) {
			unsigned int tileId;
			if (y == 0 || y == m_height -1 || x == 0 || x == m_width-1) {
				tileId = 0;
				if ((y == 0 && x == 0) || (y == m_height - 1 && x == m_width - 1) || (y == m_height - 1 & x == 0) || (y == 0 && x == m_width - 1)) {

				}
				else {
					if (doorDist(m_randomEngine) == 1) {
						tileId = 3;
						numDoors++;
						m_doors.push_back(std::make_pair(m_nextId++, x + y * m_width));
						printf("Added door with level id %i, list id %i\n", m_nextId - 1, m_doors.back().second);
					}
					if (x == enterInverse.x && y == enterInverse.y) {
						tileId = 3;
						numDoors++;
						m_doors.push_back(std::make_pair(m_id, x + y * m_width));
						printf("Added back door with level id %i, list id %i\n", m_doors.back().first, m_doors.back().second);
					}
				}
			}
			else {
				if (rubbleDist(m_randomEngine) == 1) {
					tileId = 2;
				}
				else {
					tileId = 1;
				}
			}
			m_tiles.push_back(TileRegistry::getTileFromId(tileId));
		}
	}

	if (numDoors < 4) {
		m_level--;
		m_nextId -= numDoors;
		printf("Failed level! Trying again...\n");
		generateLevel(newId);
	}

	std::uniform_int_distribution<int> positionX(1, m_width-2);
	std::uniform_int_distribution<int> positionY(1, m_height-2);
	std::uniform_int_distribution<int> entityid(0, (int)(std::min(difficulty, EntitySelector::getNumEntities() - 1)));

	for (size_t i = 0; i < numEnemies; i++) {
		int posX = positionX(m_randomEngine), posY = positionY(m_randomEngine);
		while (areaClippable(posX, posY)) {
			posX = positionX(m_randomEngine);
			posY = positionY(m_randomEngine);
		}
		m_entities.push_back(EntitySelector::getEntity(entityid(m_randomEngine)));
		m_entities.back()->position = glm::ivec2(posX, posY);
	}
	m_id = newId;
	renderTiles();

	m_numEnemies = m_entities.size();
}

void Level::saveLevel()
{
	std::ofstream file("save/leveldata.dat", std::ios::binary | std::ios::app);
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	//Test file
	if (file.fail()) {
		return;
	}

	file.seekp(std::ios::beg);
	size_t length;
	file.seekp(0, file.end);
	length = file.tellp();

	unsigned int begPtr = file.tellp();

	//End of science

	std::vector<unsigned char> levelData;

	//God help us all

	levelData.insert(levelData.end(), (unsigned char*)(&m_id), (unsigned char*)(&m_id) + 4);
	levelData.insert(levelData.end(), (unsigned char*)(&m_level), (unsigned char*)(&m_level) + 4);
	levelData.insert(levelData.end(), (unsigned char*)(&m_width), (unsigned char*)(&m_width) + 4);
	levelData.insert(levelData.end(), (unsigned char*)(&m_height), (unsigned char*)(&m_height) + 4);

	for (Tile* tile : m_tiles) {
		levelData.insert(levelData.end(), (unsigned char*)(&tile->id), (unsigned char*)(&tile->id) + 4);
	}

	unsigned int numDoors = m_doors.size();
	levelData.insert(levelData.end(), (unsigned char*)(&numDoors), (unsigned char*)(&numDoors) + 4);

	for (auto& it : m_doors) {
		levelData.insert(levelData.end(), (unsigned char*)(&it.first), (unsigned char*)(&it.first) + 4);
		levelData.insert(levelData.end(), (unsigned char*)(&it.second), (unsigned char*)(&it.second) + 4);
	}
	
	file.write((char*)(levelData.data()), levelData.size());

	printf("Saved level with %i chars of data. %i width, %i height. beg ptr %i, end ptr %i\n", levelData.size(), m_width, m_height, begPtr, (int)file.tellp());
}

void Level::loadLevel(unsigned int id)
{
	std::ifstream file("save/leveldata.dat", std::ios::binary);

	if (file.fail()) {
		generateLevel(id);
		return;
	}

	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	size_t length;
	file.seekg(0, file.end);
	length = file.tellg();
	file.seekg(0, file.beg);

	if (length < 16) {
		generateLevel(id);
		return;
	}

	cleanData();

	unsigned int ptrId = UINT_MAX;
	while (ptrId != id && file.tellg() < length) {
		file.read((char*)(&ptrId), 4);
		if (ptrId == id) {
			file.seekg((int)file.tellg() - 4);
			//printf("Found level with id %i, in file at ptr %i\n", ptrId, (int)file.tellg());
			continue;
		}
		if (!file) {
			generateLevel(id);
			file.close();
			return;
		}
		//printf("Seeking for ID %i, found %i at %i\n", id, ptrId, (int)file.tellg());
		unsigned int width, height;
		file.seekg((unsigned int)file.tellg() + 4);
		file.read((char*)(&width), 4);
		file.read((char*)(&height), 4);
		file.seekg((unsigned int)file.tellg() + ((width * height) * 4));
		unsigned int numDoors;
		file.read((char*)(&numDoors), 4);
		file.seekg((unsigned int)file.tellg() + (8 * numDoors));
	}

	if (ptrId != id) {
		generateLevel(id);
		return;
	}

	unsigned int begPtr = file.tellg();

	std::vector<unsigned int> levelData;

	file.read((char*)(&m_id), 4);
	file.seekg((unsigned int)file.tellg());
	file.read((char*)(&m_level), 4);
	file.seekg((unsigned int)file.tellg());
	file.read((char*)(&m_width), 4);
	file.seekg((unsigned int)file.tellg());
	file.read((char*)(&m_height), 4);
	file.seekg((unsigned int)file.tellg());
	//printf("Found level %i with width %i, height %i\n", id, m_width, m_height);

	levelData.resize(m_width * m_height);
	unsigned int numDoors;

	file.read((char*)(&levelData[0]), m_width * m_height * 4);

	for (unsigned int id : levelData) {
		m_tiles.push_back(TileRegistry::getTileFromId(id));
	}

	file.read((char*)(&numDoors), 4);
	for (size_t i = 0; i < numDoors; i++) {
		unsigned int f, s;
		file.read((char*)(&f), 4);
		file.read((char*)(&s), 4);

		m_doors.push_back(std::make_pair(f, s));
	}

	m_cleared = true;
	printf("Loaded level id %i, with %i chars of data, %i width, %i height, beg ptr %i, end ptr %i\n", ptrId, (int)file.tellg() - begPtr, m_width, m_height, begPtr, (int)file.tellg());
	renderTiles();

	m_numEnemies = 0;
}

void Level::generateBossLevel()
{
	m_width = 21;
	m_height = 21;

	int difficulty = (int)floor(m_level / 5.f);

	for (size_t y = 0; y < m_height; y++) {
		for (size_t x = 0; x < m_width; x++) {
			unsigned int tileId = 1;

			if (x == 0 || y == 0 || x == m_width - 1 || y == m_height - 1) {
				tileId = 0;

				if ((x == 10 && y == 0) || (x == 0 && y == 10) || (x == 10 && y == m_height - 1) || (x == m_width - 1 && y == 10)) {
					tileId = 3;
					m_doors.emplace_back(m_nextId, x + y * 21);
				}
			}

			m_tiles.push_back(TileRegistry::getTileFromId(tileId));
		}
	}

	m_entities.push_back(EntitySelector::getBossEntity((difficulty-1) % EntitySelector::getNumEntities()));
	m_entities.back()->position = glm::ivec2(10);
	
	m_boss = true;

	renderTiles();
}

void Level::generateTitleScreen()
{
	cleanData();

	std::string tileData = "#####.####.#.#.#.#####.###..###..####.#...#.#####.#...##.....#....#.#.#...#...#..#.#..#.#....##..#...#...#...##.###.####.#.#.#...#...###..###..####.#.#.#...#...######...#.#....#.#.#...#...#..#.#..#.#....#..##...#...#...######.####.#####.#####.#..#.#..#.####.#...#...#...#...#....................................................................WWWWWWWWWWWWWWWWWWWWWWWWWWWW...........................WD.............@..........DW...........................WWWWWWWWWWWWWWWWWWWWWWWWWWWW.....................................................................###.#...#..#.#..........................##..#.#.###.######.#..#.#..#..........................#..#.#.#..#...#.#...#..###..#..........................#..#.#.#..#...#.#...##.#.#..#...........................####.#..###..#.";

	for(int y = 13; y >= 0; y--)
	for (int x = 0; x < 55; x++) {
		unsigned int index = x + y * 55;
		unsigned int id = 1;
		if (tileData[index] == '#') id = 4;
		if (tileData[index] == 'W') id = 0;
		if (tileData[index] == '.') id = 1;
		if (tileData[index] == 'D') id = 3;
		if (tileData[index] == '@') {
			Base::getGame()->getPlayer()->setPosition(glm::ivec2(x-1, y-1));
			id = 1;
		}
		m_tiles.push_back(TileRegistry::getTileFromId(id));
	}

	printf("Num tiles: %i\n", m_tiles.size());

	m_width = 55;
	m_height = 14;

	m_cleared = true;
	renderTiles();

	//m_doors.emplace_back(15 + 7 * 50, 0);
	//314, 334

	m_doors.emplace_back(0xFFFFFFFF, 344);
	m_doors.emplace_back(0xFFFFFFFE, 369);

	Base::getGame()->setScaleLimit(glm::vec2(0.185f, 0.185f));
}

void Level::update()
{
	for (Entity* entity : m_entities) {
		entity->update();
		EntityRenderer::addEntity(entity);
	}
	if (m_numEnemies == 0 && m_cleared == false) {
		for (Tile* tile : m_tiles) {
			tile->levelCleared();
		}
		m_cleared = true;
		renderTiles();
		if (!m_boss) {
			saveLevel();
		}
		Base::getGame()->getPlayer()->save();
	}
}

void Level::render()
{
	TileRenderer::render();
	EntityRenderer::render();
}

Tile * Level::getTile(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return nullptr;
	if (x + y * m_width >= m_tiles.size()) return nullptr;
	return m_tiles[x + y * m_width];
}

Entity * Level::getEntity(int x, int y)
{
	for (Entity* entity : m_entities) {
		if (entity->position.x == x && entity->position.y == y) {
			return entity;
		}
	}
	return nullptr;
}

bool Level::areaClippable(int x, int y)
{
	if (getTile(x, y) == nullptr) { return false;}
	if (getTile(x, y)->isSolid()) { return false;}
	if (Base::getGame()->getPlayer()->getPosition() == glm::ivec2(x, y)) { return false;}
	for (Entity* entity : m_entities) {
		if (entity->position == glm::ivec2(x, y)) {return false;}
	}
	return true;
}

void Level::addEntity(Entity * entity)
{
	m_entities.push_back(entity);
}

void Level::killEntity(Entity * entity)
{
	for (size_t i = 0; i < m_entities.size(); i++) {
		if (m_entities[i] == entity) {
			m_entities.erase(m_entities.begin() + i);
			delete entity;
			entity = 0;
			return;
		}
	}
}

void Level::enterDoorFrom(glm::ivec2 & position)
{
	m_persistWidth = false;
	m_persistHeight = false;
	if (position.x == 0 || position.x == m_width - 1) {
		m_persistHeight = true;
	}
	if (position.y == 0 || position.y == m_height - 1) {
		m_persistWidth = true;
	}

	m_enterPos = position;

	for (auto& it : m_doors) {
		if (it.second == position.x + position.y * m_width) {
			if (it.first == 0xFFFFFFFF) {
				Base::getGame()->getPlayer()->load();
				Base::getGame()->setScaleLimit(glm::vec2(0.2f, 1.f));
				loadLevel(m_currentLevel);
				return;
			}
			if (it.first == 0xFFFFFFFE) {
				Base::requestExit();
				return;
			}
			printf("Entered door to level id %i\n", it.first);
			int oldWidth = m_width, oldHeight = m_height;
			loadLevel(it.first);

			Base::getGame()->getPlayer()->setPosition(glm::ivec2(
				m_enterPos.x == 0 ? m_width - 2 : (m_enterPos.x == oldWidth - 1 ? 1 : Base::getGame()->getPlayer()->getPosition().x),
				m_enterPos.y == 0 ? m_height - 2 : (m_enterPos.y == oldHeight - 1 ? 1 : Base::getGame()->getPlayer()->getPosition().y)
			));
			if (m_boss) {
				m_boss = false;
				Base::getGame()->getPlayer()->setPosition(glm::ivec2(1));
			}
			return;
		}
	}

	printf("Failed to load door %i in list\n", position.x + position.y * m_width);
}

void Level::renderTiles()
{
	TileRenderer::addTilesForBatching(m_tiles, m_width, m_height);
}

void Level::cleanData()
{
	for (std::vector<Entity*>::iterator it = m_entities.begin(); it != m_entities.end();) {
		(*it)->clean();
		delete (*it);
		it = m_entities.erase(it);
	}

	m_tiles.clear();

	m_entities.clear();

	m_doors.clear();
}
