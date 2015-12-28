#include <State/HighscoreState.h>
#include <Core/GameEngine.h>
#include <Utils/Utils.h>
#include <Utils/Log.h>

std::string getHighscoreFile()
{
  return Utils::getAppDataDirectory("highscore.txt");
}

bool HighscoreState::init()
{
	internalState = SHOW;
	inputText = new Text(game->getRenderer(), "Enter your name", 167, 203, 237, 0, 0, game->standard_font_46);
	highscoreScreen = new Sprite(game, "./gfx/menu/highscore.png", 0, 0, WWIDTH, WHEIGHT);
	gameoverScoreText = new Text(game->getRenderer(), "", 167, 203, 237, 0, 0, game->standard_font_42);
	return true;
}

void HighscoreState::cleanup()
{
	delete highscoreScreen;
	delete inputText;
	delete gameoverScoreText;

	for (HighscoreList::iterator it = highscores.begin(); it != highscores.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<Sprite*>::iterator it = highscoreNameSprites.begin(); it != highscoreNameSprites.end(); it++)
	{
		delete (*it);
	}

	for (std::vector<Sprite*>::iterator it = highscoreScoreSprites.begin(); it != highscoreScoreSprites.end(); it++)
	{
		delete (*it);
	}
}

void HighscoreState::onEnter()
{
	if (internalState == ENTER)
	{
		SDL_StartTextInput();
	}
}

void HighscoreState::onExit()
{
	SDL_StopTextInput();
}

bool HighscoreState::handleEvent(const SDL_Event &ev)
{
	if (ev.key.type != SDL_KEYDOWN && ev.type != SDL_TEXTINPUT)
	{
		return false;
	}

	if (internalState == SHOW)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			game->getEngine()->setState((State*)game->mainMenuState);
			internalState = SHOW;
			return true;
		}
	}
	else if (internalState == ENTER)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			game->getEngine()->setState((State*)game->mainMenuState);
			internalState = SHOW;
			return true;
		}
		else if (ev.key.keysym.sym == SDLK_RETURN)
		{
			if (playerName != "")
			{
				insertNewHighscore(game->getScore(), getHighscorePos(game->getScore()), playerName);
				writeHighscoreToFile();
				updateHighscoreSprites();

				internalState = SHOW;

				inputText->update_text("Enter your name");
				return true;
			}
		}
		else if (ev.key.keysym.sym == SDLK_BACKSPACE)
		{
			if (playerName.length() != 0)
			{
				playerName.erase(playerName.length() - 1);
				inputText->update_text(playerName + "_");
			}
			return true;
		}
		else if (ev.type == SDL_TEXTINPUT)
		{
			if (playerName.length() < PLAYERNAME_LENGTH_MAX - 1)
			{
				playerName += ev.text.text;
				inputText->update_text(playerName + "_");
				return true;
			}
			else if (playerName.length() == PLAYERNAME_LENGTH_MAX - 1)
			{
				playerName += ev.text.text;
				inputText->update_text(playerName);
				return true;
			}

		}
	}
	return false;
}

void HighscoreState::render(SDL_Renderer* const renderer)
{
	highscoreScreen->draw(renderer);

	if (internalState == SHOW)
	{
		int y_start = 190;
		int row_height = 0;

		int offset_L = 150;
		int offset_R = WWIDTH - offset_L;

		int counter = 1;
		std::vector<Sprite*>::iterator iter_highscore_name = highscoreNameSprites.begin();
		std::vector<Sprite*>::iterator iter_highscore_score = highscoreScoreSprites.begin();

		while (iter_highscore_name != highscoreNameSprites.end() && iter_highscore_score != highscoreScoreSprites.end())
		{
			if (counter == 1)
				row_height += 0;

			else if (counter == 2)
			{
				row_height += 60;
				offset_L += 40;
				offset_R = 800 - offset_L;
			}
			else
			{
				row_height += 30;
				offset_L += 25 - (counter * 2);
				offset_R = WWIDTH - offset_L;
			}

			(*iter_highscore_name)->draw(renderer, offset_L, y_start + row_height);
			(*iter_highscore_score)->draw(renderer, (offset_R - (*iter_highscore_score)->get_width()), y_start + row_height);

			counter++;
			iter_highscore_name++;
			iter_highscore_score++;
		}
	}
	else if (internalState == ENTER)
	{
		gameoverScoreText->draw(renderer, ((WWIDTH / 2) - (gameoverScoreText->get_width() / 2)), 200);
		inputText->draw(renderer, ((WWIDTH / 2) - (inputText->get_width() / 2)), 270);
	}
}


void HighscoreState::updateHighscoreSprites()
{
	// Delete earlier sprites
	if (!highscoreNameSprites.empty())
	{
		for (std::vector<Sprite*>::iterator it = highscoreNameSprites.begin(); it != highscoreNameSprites.end(); it++)
		{
			delete (*it); (*it) = NULL;
		}
	}

	if (!highscoreScoreSprites.empty())
	{
		for (std::vector<Sprite*>::iterator it = highscoreScoreSprites.begin(); it != highscoreScoreSprites.end(); it++)
		{
			delete (*it);	(*it) = NULL;
		}
	}
	
	highscoreNameSprites.clear();
	highscoreScoreSprites.clear();

	readHighscoresFromFile();

	std::string tmp_name = "";
	std::string tmp_score = "";

	int list_num = 1;
	HighscoreList::iterator it = highscores.begin();
	while (it != highscores.end())
	{
		{
			std::ostringstream oss_name;
			oss_name << Utils::itos(list_num) << ". " << (*it)->second;
			tmp_name = oss_name.str();

			std::ostringstream oss_score;
			oss_score << (*it)->first;
			tmp_score = oss_score.str();
		}
		if (list_num == 1)
		{
			highscoreNameSprites.push_back(new Text(game->getRenderer(), tmp_name, 186, 255, 246, 0, 0, game->standard_font_48));
			highscoreScoreSprites.push_back(new Text(game->getRenderer(), tmp_score, 186, 255, 246, 0, 0, game->standard_font_48));
		}
		else
		{
			highscoreNameSprites.push_back(new Text(game->getRenderer(), tmp_name, 167, 203, 237, 0, 0, game->standard_font_20));
			highscoreScoreSprites.push_back(new Text(game->getRenderer(), tmp_score, 167, 203, 237, 0, 0, game->standard_font_20));
		}

		list_num++;
		it++;
	}
}

const int HighscoreState::getHighscorePos(const int score)
{
	if(highscores.empty())
	{
		readHighscoresFromFile();
		if (highscores.empty())
		{
			return 0;
		}
	}

	for (unsigned int i = 0; i < 10 && i < highscores.size(); i++)
	{
		if (score > highscores[i]->first)
		{
			return i;
		}
	}

	if(highscores.size() < 10)
	{
		return highscores.size();
	}
	else
	{
		return -1;
	}
}

bool HighscoreState::readHighscoresFromFile()
{
	LOG_DEBUG << "Reading highscores";
	if (!highscores.empty())
	{
		for (HighscoreList::iterator it = highscores.begin(); it != highscores.end(); it++)
		{
			delete (*it);
		}

		highscores.clear();
	}

	std::ifstream file_in;
	file_in.open(getHighscoreFile());
	if (!file_in)
	{
		LOG_DEBUG << "Highscore file does not exist. Using default values";
		// default
		highscores.push_back(new std::pair<int, std::string>(1000, "Uniden"));
		highscores.push_back(new std::pair<int, std::string>(500, "Bigby Wolf"));
		highscores.push_back(new std::pair<int, std::string>(250, "Dilbert"));
		highscores.push_back(new std::pair<int, std::string>(125, "Ananakinin"));
		highscores.push_back(new std::pair<int, std::string>(50, "Here's Johnny"));
		highscores.push_back(new std::pair<int, std::string>(40, "Lady Gaga"));
		highscores.push_back(new std::pair<int, std::string>(30, "Tomten"));
		highscores.push_back(new std::pair<int, std::string>(20, "Red Rose"));
		highscores.push_back(new std::pair<int, std::string>(10, "Blue Screen"));
		highscores.push_back(new std::pair<int, std::string>(0, "Lab-ASS"));
		return true;
	}

	int score;
	std::string name;

	file_in >> score;
	file_in.get();
	LOG_DEBUG << "Reading highscore entries from file";
	while (getline(file_in, name))
	{
		LOG_DEBUG << name << " " << score;
		highscores.push_back(new std::pair<int, std::string>(score, name));
		file_in >> score;
		file_in.get();
	}

	file_in.close();
	return true;
}

void HighscoreState::insertNewHighscore(int newScore, int position, std::string name)
{
	LOG_DEBUG << "Inserting highscore(" << name << " " << newScore << " at position " << position;
	HighscoreList::iterator it = highscores.begin();
	it += position;

	highscores.insert(it, new std::pair<int, std::string>(newScore, name));
}

void HighscoreState::writeHighscoreToFile()
{
	LOG_DEBUG << "Writing highscores to file";
	std::ofstream fout;
	fout.open(getHighscoreFile());

	int score_count = 0;
	for (HighscoreList::iterator it = highscores.begin(); it != highscores.end(); it++)
	{
		if (score_count++ == 10)
		{
			break;
		}

		LOG_DEBUG << (*it)->first << " " << (*it)->second;
		fout << (*it)->first << " " <<(*it)->second << std::endl;
	}
}

bool HighscoreState::isQualifyingScore(const int score)
{
	return getHighscorePos(score) != -1;
}

void HighscoreState::setScore(const int score)
{
	std::string str = "Your score: ";
	str.append(Utils::itos(score));
	gameoverScoreText->update_text(str);
}