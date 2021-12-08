#include "entity.h"

size_t Entity::counter = 0;
std::vector<std::shared_ptr<Entity>> Entity::entities;

void Entity::initEntitySystem() {
    entities.reserve(5096);
    counter = 0;
}

void Entity::clearEntities() {
    entities.clear();
    initEntitySystem();
}

void Entity::destroyEntity(std::shared_ptr<Entity> entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    if(it != entities.end()) it->reset();
}

std::shared_ptr<Entity> Entity::pointCheckCollision(float x, float y) {
    olc::vf2d point = {x, y};

    for(std::shared_ptr<Entity> e : entities){
        olc::vf2d other_topleft = e->position + e->collisionBox.offset,
              other_bottomright = other_topleft + e->collisionBox.size;

        if(other_topleft.x < point.x && other_bottomright.x > point.x &&
           other_topleft.y < point.y && other_bottomright.y > point.y){ // intersects
            return e;
        }
    }

    return nullptr;
}

std::shared_ptr<Entity> Entity::findEntity(size_t id) {
    auto it = std::find_if(entities.begin(), entities.end(), [&](std::shared_ptr<Entity> e) -> bool {
        if(!e) return false;
        return e->id == id;
    });

    if(it == entities.end()) return nullptr;
    return *it;
}

Entity::Entity(): position({0,0}), color(0xFFFFFFFF), collisionBox({{-16, -16}, {32, 32}}) {
    id = counter++;
}

Entity::~Entity() {}

size_t Entity::checkCollision(std::vector<std::weak_ptr<Entity>>& list) {
    list.clear(); // clear list for fresh list of collision checks
    olc::vf2d topleft = position + collisionBox.offset,
              bottomright = topleft + collisionBox.size;

    for(std::shared_ptr<Entity> e : entities){
        if(!e || e.get() == this) continue;
        olc::vf2d other_topleft = e->position + e->collisionBox.offset,
              other_bottomright = other_topleft + e->collisionBox.size;

        if(other_topleft.x < bottomright.x && other_bottomright.x > topleft.x &&
           other_topleft.y < bottomright.y && other_bottomright.y > topleft.y){ // intersects
            list.push_back(e);
        }

    }

    return list.size();
}

void Entity::draw() {
    pge->FillRect(position + collisionBox.offset, collisionBox.size, color);
}

void Entity::update(float delta) {}