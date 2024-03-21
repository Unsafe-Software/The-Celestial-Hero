#include "entity.hh"

namespace Engine {
    namespace Entities {
        std::vector<Rectangle> GetPossibleCollidingTiles(World::World* world, Rectangle bounds) {
            std::vector<Rectangle> collidingTiles = {};
            for (int x = round(bounds.x) - 2; x < round(bounds.x) + 3; x++) {
                for (int y = round(bounds.y) - 2; y < round(bounds.y) + 4; y++) {
                    if (x < 0 || y < 0 || x >= world->world_size_x * world->Data[0][0]->chunk_size_x ||
                        y >= world->world_size_y * world->Data[0][0]->chunk_size_y) {
                        continue;
                    }
                    if (isSolid(world->GetCell(y, x))) collidingTiles.push_back({x * 16.0f, y * 16.0f, 16.0f, 16.0f});
                }
            }
            return collidingTiles;
        }

        Entity::Entity() {
            this->bounds = {0.0f, 0.0f, 16.0f, 16.0f};
            this->velocity = {0.0f, 0.0f};
            this->world = nullptr;
            this->noClip = false;
            this->texture = {0};
        }

        Entity::Entity(World::World* world, Texture2D texture, bool noClip) {
            this->bounds = {0.0f, 0.0f, 16.0f, 16.0f};
            this->velocity = {0.0f, 0.0f};
            this->world = world;
            this->noClip = noClip;
            this->texture = texture;
        }

        Entity::~Entity() {}

        void Entity::updatePosition(bool debug) {
            Vector2 offset = {0.0f, 0.0f};
            offset.x += this->velocity.x;
            offset.y += this->velocity.y;
            if (offset.x != 0 && offset.y != 0) {
                offset.x = offset.x / 1.4142;
                offset.y = offset.y / 1.4142;
            }
            if (noClip == false) {
                Vector2 newPos = {this->bounds.x + offset.x, this->bounds.y + offset.y};
                this->ResolveWorldCollisions(newPos, debug);
                if (this->bounds.x < 0) this->bounds.x = 0;
                if (this->bounds.y < 0) this->bounds.y = 0;
                if (this->bounds.x > this->world->world_size_x * this->world->Data[0][0]->chunk_size_x - this->bounds.width)
                    this->bounds.x = this->world->world_size_x * this->world->Data[0][0]->chunk_size_x - this->bounds.width;
                if (this->bounds.y > this->world->world_size_y * this->world->Data[0][0]->chunk_size_y - this->bounds.height)
                    this->bounds.y = this->world->world_size_y * this->world->Data[0][0]->chunk_size_y - this->bounds.height;
            } else {
                this->bounds.x += offset.x;
                this->bounds.y += offset.y;
            }
            this->lastVelocity = this->velocity;
            this->velocity = {0.0f, 0.0f};
        }

        void Entity::AddForce(Vector2 force) { this->velocity = Vector2Add(this->velocity, force); }

        void Entity::Update(bool debug) {}

        void Entity::Draw(int tile_size) {
            DrawTexturePro(this->texture, (Rectangle){0.0f, 0.0f, (float)this->texture.width, (float)this->texture.height},
                (Rectangle){this->bounds.x * tile_size, this->bounds.y * tile_size, this->bounds.width * tile_size, this->bounds.height * tile_size},
                (Vector2){0, 0}, 0.0f, WHITE);
        }

        void Entity::ResolveWorldCollisions(const Vector2 newPlayerPos, bool debug) {
            std::vector<Rectangle> collidingTiles = GetPossibleCollidingTiles(this->world, this->bounds);
            Vector2 resultPlayerPos = {newPlayerPos.x * 16.0f, newPlayerPos.y * 16.0f};
            for (Rectangle tile : collidingTiles) {
                resultPlayerPos = this->ResolveCollisionBox(
                    (Rectangle){this->bounds.x * 16.0f, this->bounds.y * 16.0f, this->bounds.width * 16.0f, this->bounds.height * 16.0f}, resultPlayerPos, tile,
                    debug);
            }
            this->bounds.x = resultPlayerPos.x / 16.0f;
            this->bounds.y = resultPlayerPos.y / 16.0f;
        }

        Vector2 Entity::ResolveCollisionBox(const Rectangle player, const Vector2 newPlayerPos, const Rectangle box, bool debug) {
            Vector2 resultPlayerPos = {newPlayerPos.x, newPlayerPos.y};
            Ray ray = {0};
            ray.position = (Vector3){player.x + player.width / 2.0f, player.y + player.height / 2.0f, 0.0f};
            ray.direction = Vector3Normalize(Vector3Subtract({newPlayerPos.x, newPlayerPos.y, 0.0f}, {player.x, player.y, 0.0f}));
            if (debug) DrawRay(ray, RED);
            Rectangle oversizedBox = {box.x - player.width / 2.0f, box.y - player.height / 2.0f, box.width + player.width, box.height + player.height};
            RayCollision collision = GetRayCollisionBox(ray, BoundingBox{(Vector3){oversizedBox.x, oversizedBox.y, 0.0f},
                                                                 (Vector3){oversizedBox.x + oversizedBox.width, oversizedBox.y + oversizedBox.height, 0.0f}});

            if (collision.hit == 1) {
                float maxDistanceX = Vector2Distance(
                    {player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {newPlayerPos.x + player.width / 2.0f, player.y + player.height / 2.0f});
                float DistanceX =
                    Vector2Distance({player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {collision.point.x, player.y + player.height / 2.0f});

                if (DistanceX == 0) {
                    Ray rayHor = {0};
                    rayHor.position = (Vector3){player.x + player.width / 2.0f, player.y + player.height / 2.0f, 0.0f};
                    rayHor.direction = Vector3Normalize(Vector3Subtract({newPlayerPos.x, 0.0f, 0.0f}, {player.x, 0.0f, 0.0f}));
                    RayCollision collisionHor =
                        GetRayCollisionBox(rayHor, BoundingBox{(Vector3){oversizedBox.x, oversizedBox.y, 0.0f},
                                                       (Vector3){oversizedBox.x + oversizedBox.width, oversizedBox.y + oversizedBox.height, 0.0f}});
                    if (collisionHor.hit == 1) {
                        float maxDistanceHor = Vector2Distance({player.x + player.width / 2.0f, player.y + player.height / 2.0f},
                            {newPlayerPos.x + player.width / 2.0f, player.y + player.height / 2.0f});
                        float DistanceHor = Vector2Distance(
                            {player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {collisionHor.point.x, player.y + player.height / 2.0f});
                        if (DistanceHor < maxDistanceHor) {
                            resultPlayerPos.x = collisionHor.point.x - player.width / 2.0f;
                        }
                    }
                } else if (DistanceX < maxDistanceX) {
                    resultPlayerPos.x = collision.point.x - player.width / 2.0f;
                }

                float maxDistanceY = Vector2Distance(
                    {player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {player.x + player.width / 2.0f, newPlayerPos.y + player.height / 2.0f});
                float DistanceY =
                    Vector2Distance({player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {player.x + player.width / 2.0f, collision.point.y});
                if (DistanceY == 0) {
                    Ray rayVer = {0};
                    rayVer.position = (Vector3){player.x + player.width / 2.0f, player.y + player.height / 2.0f, 0.0f};
                    rayVer.direction = Vector3Normalize(Vector3Subtract({0.0f, newPlayerPos.y, 0.0f}, {0.0f, player.y, 0.0f}));
                    RayCollision collisionVer =
                        GetRayCollisionBox(rayVer, BoundingBox{(Vector3){oversizedBox.x, oversizedBox.y, 0.0f},
                                                       (Vector3){oversizedBox.x + oversizedBox.width, oversizedBox.y + oversizedBox.height, 0.0f}});
                    if (collisionVer.hit == 1) {
                        float maxDistanceVer = Vector2Distance({player.x + player.width / 2.0f, player.y + player.height / 2.0f},
                            {player.x + player.width / 2.0f, newPlayerPos.y + player.height / 2.0f});
                        float DistanceVer = Vector2Distance(
                            {player.x + player.width / 2.0f, player.y + player.height / 2.0f}, {player.x + player.width / 2.0f, collisionVer.point.y});
                        if (DistanceVer < maxDistanceVer) {
                            resultPlayerPos.y = collisionVer.point.y - player.height / 2.0f;
                        }
                    }
                } else if (DistanceY < maxDistanceY) {
                    resultPlayerPos.y = collision.point.y - player.height / 2.0f;
                }
            }

            return resultPlayerPos;
        }
    }  // namespace Entities
}  // namespace Engine
