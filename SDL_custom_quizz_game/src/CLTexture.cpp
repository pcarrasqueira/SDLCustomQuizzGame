#include <CLTexture.h>


CLTexture::CLTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

CLTexture::~CLTexture()
{
	//Deallocate
	free();
}

bool CLTexture::loadFromFile(std::string path, SDL_Renderer* Renderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool CLTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* Font, SDL_Renderer* Renderer)
{
	//Get rid of preexisting texture
	free();
	if (Font == NULL || Renderer == NULL)
	{
		//Invalide parameters
		return false;
	}
	else
	{
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(Font, textureText.c_str(), textColor);
		if (textSurface != NULL)
		{
			//Create texture from surface pixels
			mTexture = SDL_CreateTextureFromSurface(Renderer, textSurface);
			if (mTexture == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Get image dimensions
				mWidth = textSurface->w;
				mHeight = textSurface->h;
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}
		else
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		//Return success
		return mTexture != NULL;
	}
}
#endif

void CLTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void CLTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void CLTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void CLTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void CLTexture::render(SDL_Renderer* Renderer, int x, int y, int Width, int Height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	if (Renderer == NULL)
	{
		// Invalide Render
	}
	else
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad;
		if (Width != NULL && Height != NULL)
		{
			renderQuad = { x, y, Width, Height };
			mWidth = Width;
			mHeight = Height;
		}
		else
		{
			renderQuad = { x, y, mWidth, mHeight };
		}

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx(Renderer, mTexture, clip, &renderQuad, angle, center, flip);
	}
}

int CLTexture::getWidth()
{
	return mWidth;
}

int CLTexture::getHeight()
{
	return mHeight;
}