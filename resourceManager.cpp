#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "resourceManager.hpp"
#include "entity.hpp"

ResourceManager *ResourceManager::s_pInstance = nullptr;

bool ResourceManager::init(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "HEY... SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_JPG)))
    {
        std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (m_pWindow == nullptr)
    {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // Virtual machine
    // renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(m_pWindow));
    
    if (m_pRenderer == nullptr)
    {
        std::cout << "Renderer failed to init. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* ResourceManager::loadTexture(const char* filePath)
{
    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(m_pRenderer, filePath);

    if (texture == nullptr)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
    return texture;
}

int ResourceManager::getRefreshRate()
{
    int displayIndex = SDL_GetWindowDisplayIndex(m_pWindow);

    SDL_DisplayMode mode;

    SDL_GetDisplayMode(displayIndex, 0, &mode);

    return mode.refresh_rate;
}

void ResourceManager::clean()
{
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
}

void ResourceManager::clear()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);
}

void ResourceManager::draw(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;
    
    SDL_Rect dst;
    dst.x = entity.getPos().getX();
    dst.y = entity.getPos().getY();
    dst.w = entity.getCurrentFrame().w;
    dst.h = entity.getCurrentFrame().h;
    
    SDL_RenderCopy(m_pRenderer, entity.getTex(), NULL, &dst);
}

void ResourceManager::drawFrame(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().w * entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().h * entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;
    
    SDL_Rect dst;
    dst.x = entity.getPos().getX();
    dst.y = entity.getPos().getY();
    dst.w = entity.getCurrentFrame().w;
    dst.h = entity.getCurrentFrame().h;
    
    if (entity.getFlip()) SDL_RenderCopyEx(m_pRenderer, entity.getTex(), &src, &dst, 0, 0, SDL_FLIP_HORIZONTAL);
    else SDL_RenderCopy(m_pRenderer, entity.getTex(), &src, &dst);
}

void ResourceManager::drawPlayer(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().w * entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().h * entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getPos().getX();
    dst.y = entity.getPos().getY();
    dst.w = entity.getCurrentFrame().w * 2;
    dst.h = entity.getCurrentFrame().h * 2;

    if (entity.getFlip()) SDL_RenderCopyEx(m_pRenderer, entity.getTex(), &src, &dst, 0, 0, SDL_FLIP_HORIZONTAL);
    else SDL_RenderCopy(m_pRenderer, entity.getTex(), &src, &dst);
}

void ResourceManager::drawEnemy1(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().w * entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().h * entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getPos().getX();
    dst.y = entity.getPos().getY();
    dst.w = entity.getCurrentFrame().w * 3;
    dst.h = entity.getCurrentFrame().h * 3;

    if (entity.getFlip()) SDL_RenderCopyEx(m_pRenderer, entity.getTex(), &src, &dst, 0, 0, SDL_FLIP_HORIZONTAL);
    else SDL_RenderCopy(m_pRenderer, entity.getTex(), &src, &dst);
}

void ResourceManager::drawEnemy2(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().w * entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().h * entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = entity.getPos().getX();
    dst.y = entity.getPos().getY();
    dst.w = entity.getCurrentFrame().w;
    dst.h = entity.getCurrentFrame().h;

    if (entity.getFlip()) SDL_RenderCopyEx(m_pRenderer, entity.getTex(), &src, &dst, 0, 0, SDL_FLIP_HORIZONTAL);
    else SDL_RenderCopy(m_pRenderer, entity.getTex(), &src, &dst);
}

void ResourceManager::display()
{
    SDL_RenderPresent(m_pRenderer);
    // Virtual machine
    // SDL_UpdateWindowSurface(m_pWindow);
}