#include "./entity.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "../world/world.hpp"

#define SLOW_DOWN 0.01
#define MAX_SPEED 0.25

namespace Entities {
    bool PointRectCollision(Vector2 Point, Rectangle Rect) {
        return (
            Point.x >= Rect.x && Point.y >= Rect.y &&
            Point.x < Rect.x + Rect.width &&
            Point.y < Rect.y + Rect.height
        );
    }

    bool RayRectCollision(
        const Vector2 ray_origin, const Vector2 ray_dir,
        const Rectangle target, Vector2 &contact_point, Vector2 &contact_normal, float &t_hit_near
    ) {
        contact_normal = { 0,0 };
        contact_point = { 0,0 };

        Vector2 invdir = Vector2Divide((Vector2){1.0f}, ray_dir);
        Vector2 t_pos = (Vector2){target.x, target.y};
        Vector2 t_size = (Vector2){target.width, target.height};
        Vector2 t_near = Vector2Multiply(Vector2Subtract(t_pos, ray_origin), invdir);
        Vector2 t_far = Vector2Multiply(Vector2Subtract(Vector2Add(t_pos, t_size), ray_origin), invdir);

        if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
        if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

        if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
        if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

        if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

        t_hit_near = std::max(t_near.x, t_near.y);

        float t_hit_far = std::min(t_far.x, t_far.y);

        if (t_hit_far < 0)
            return false;

        contact_point = Vector2Add(ray_origin, Vector2Multiply((Vector2){t_hit_near, t_hit_near}, ray_dir));

        if (t_near.x > t_near.y) {
            if (invdir.x < 0)
                contact_normal = { 1, 0 };
            else
                contact_normal = { -1, 0 };
        }
        else if (t_near.x < t_near.y) {
            if (invdir.y < 0)
                contact_normal = { 0, 1 };
            else
                contact_normal = { 0, -1 };
        }

        return true;
    }

    bool RectsDynamicCollision(Rectangle in, Vector2 speed, Rectangle col, Vector2 &contact_point, Vector2 &contact_normal, float &contact_time) {
        Rectangle expanded_col;
        expanded_col.x = col.x;
        expanded_col.y = col.y;
        expanded_col.width = col.width + in.width;
        expanded_col.height = col.height + in.height;

        if (RayRectCollision(
            Vector2Add((Vector2){in.x, in.y},
            Vector2Divide((Vector2){in.width, in.height}, (Vector2){2.0f, 2.0f})),
            (Vector2){speed.x, speed.y}, expanded_col, contact_point, contact_normal, contact_time
        )) return contact_time >= 0.0f && contact_time < 1.0f;
        
        return false;
    }

    void Entity::resolveCollisions() {
        if (speed.x == 0 && speed.y == 0) return;
        // Rectangle col = (Rectangle){120, 60, 1, 1};
        
        // std::vector<Rectangle> rects_to_chech;
        // for (int y = floor(bound.y) - 1; y < 2 + floor(bound.y); ++y) {
        //     for (int x = floor(bound.x) - 1; x < 2 + floor(bound.x); ++x) {
        //         if (world->GetCell(y, x) == 5) {
        //             rects_to_chech.push_back((Rectangle){(float)x, (float)y, 16.0f, 16.0f});
        //             std::cout << x << "x, " << y << "y, " << std::endl;
        //         }
        //     }
        // }
        
        // bool stop = false;
        // // for (int i = 0; i < rects_to_chech.size(); ++i) {
        //     Vector2 contact_point, contact_normal;
        //     float contact_time;
        //     if (RectsDynamicCollision(
        //         bound, speed, rects_to_chech[i],
        //         contact_point, contact_normal, contact_time
        //     )) {
        //         DrawRectangle(rects_to_chech[i].x * 16, rects_to_chech[i].y * 16, 16, 16, YELLOW);
        //         speed.x = contact_normal.x * abs(speed.x) * (1 - contact_time);
        //         speed.y = contact_normal.y * abs(speed.y) * (1 - contact_time);
        //     }
        // }

        // Vector2 contact_point, contact_normal;
        // float contact_time;
        // if (RectsDynamicCollision(
        //     bound, speed, col,
        //     contact_point, contact_normal, contact_time
        // )) {
        //     speed.x = contact_normal.x * abs(speed.x) * (1 - contact_time);
        //     speed.y = contact_normal.y * abs(speed.y) * (1 - contact_time);
        // }
    }

    Entity::Entity(Rectangle Bound, Texture2D Texture, Map::World* World) {
        bound = Bound;
        texture = Texture;
        world = World;
    }

    Entity::Entity(Vector2 Position, Texture2D Texture, Map::World* World) {
        texture = Texture;
        bound = (Rectangle){Position.x, Position.y, (float)texture.width, (float)texture.height};
        world = World;
    }


    Entity::Entity(Rectangle Bound, std::string Texture_path, Map::World* World) {
        bound = Bound;
        texture = LoadTexture(Texture_path.c_str());
        world = World;
    }

    Entity::Entity(Vector2 Position, std::string Texture_path, Map::World* World) {
        texture = LoadTexture(Texture_path.c_str());
        bound = (Rectangle){Position.x, Position.y, (float)texture.width, (float)texture.height};
        world = World;
    }

    void Entity::Move(float x, float y) {
        bound.x += x;
        bound.y += y;
        resolveCollisions();
    }

    void Entity::Move(Vector2 Pos) {
        bound.x += Pos.x;
        bound.y += Pos.y;
        resolveCollisions();
    }

    void Entity::Update() {
        if (speed.x > SLOW_DOWN) {
            speed.x -= SLOW_DOWN;
        } else if (speed.x < -SLOW_DOWN) {
            speed.x += SLOW_DOWN;
        } else {
            speed.x = 0;
        }
        if (speed.y > SLOW_DOWN) {
            speed.y -= SLOW_DOWN;
        } else if (speed.y < -SLOW_DOWN) {
            speed.y += SLOW_DOWN;
        } else {
            speed.y = 0;
        }
        if (speed.x > MAX_SPEED) {
            speed.x = MAX_SPEED;
        } else if (speed.x < -MAX_SPEED) {
            speed.x = -MAX_SPEED;
        }
        if (speed.y > MAX_SPEED) {
            speed.y = MAX_SPEED;
        } else if (speed.y < -MAX_SPEED) {
            speed.y = -MAX_SPEED;
        }
        resolveCollisions();
        bound.x += speed.x;
        bound.y += speed.y;
    }

    Rectangle Entity::GetBound() {
        return bound;
    }

    Vector2 Entity::GetPos() {
        return (Vector2){bound.x, bound.y};
    }

    Vector2 Entity::GetSize() {
        return (Vector2){bound.width, bound.height};
    }

    Vector2 Entity::GetSpeed() {
        return speed;
    }

    void Entity::SetBound(Rectangle Buond) {
        bound = Buond;
        resolveCollisions();
    }

    void Entity::SetPos(Vector2 Pos) {
        bound.x = Pos.x;
        bound.y = Pos.y;
        resolveCollisions();
    }

    void Entity::SetSize(Vector2 Size) {
        bound.width = Size.x;
        bound.height = Size.y;
        resolveCollisions();
    }

    void Entity::SetSpeed(Vector2 Speed) {
        speed = Speed;
    }

    void Entity::AddToSpeed(Vector2 AddSpeed) {
        speed.x += AddSpeed.x;
        speed.y += AddSpeed.y;
    }

    void Entity::SetWorld(Map::World* NewWorld, float x, float y) {
        world = NewWorld;
        bound.x = x;
        bound.y = y;
        resolveCollisions();
    }

    void Entity::Draw(int tile_size) {
        DrawTexturePro(
            texture, (Rectangle){0.0f, 0.0f, (float)texture.width, (float)texture.height},
            (Rectangle){bound.x * tile_size, bound.y * tile_size, bound.width, bound.height},
            (Vector2){0, 0}, 0.0f, WHITE
        );
    }
}
