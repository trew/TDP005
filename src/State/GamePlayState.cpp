#include <State/GamePlayState.h>
#include <Core/GameEngine.h>
#include <Utils/Log.h>
#include <Utils/Utils.h>

bool GamePlayState::init()
{
	speedText = new Text(getRenderer(), "", 255, 255, 255, 570, 580, game->standard_font_12);
	speedText->hide();

	selectionMarker = new Sprite(game, "./gfx/misc/marker44x44.png", -5, -5, 44, 44);
	selectionMarker->hide();

	//Available towers in menu
	buildList.push_back(new BaseTower(game, towers::SIMPLE, NULL));
	buildList.back()->set_x(630);
	buildList.back()->set_y(175);
	buildList.push_back(new BaseTower(game, towers::BOOST, NULL));
	buildList.back()->set_x(680);
	buildList.back()->set_y(175);
	buildList.push_back(new BaseTower(game, towers::WALL, NULL));
	buildList.back()->set_x(730);
	buildList.back()->set_y(175);

	//Ingame buttons
	soundButton = new Button(getRenderer(), 712, 0, 44, 42, "./gfx/button/ingame-sound-44x51.png");
	inGameButtons.push_back(soundButton);
	inGameButtons.push_back(new Button(getRenderer(), 756, 0, 44, 42, "./gfx/button/ingame-gridon-44x51.png", "./gfx/button/ingame-gridoff-44x51.png", true, [this](Button* const button) -> bool
	{
		gridVisible = !gridVisible;
		button->setToggle(!gridVisible);

		return true;
	}));
	inGameButtons.push_back(new Button(getRenderer(), 647, 280, 48, 48, "./gfx/button/menu-button-upgrade-48x48.png", [this](Button* const) -> bool
	{
		if (selectedTile != NULL && selectedTowerInBuildMenu == NULL && !isOptionBoxVisible())
		{
			BaseTower* t = selectedTile->get_tower();
			if (t != NULL)
			{
				game->upgrade_tower(t);
				return true;
			}
		}

		return false;
	}));
	inGameButtons.push_back(new Button(getRenderer(), 705, 279, 48, 48, "./gfx/button/menu-button-sell-48x48.png", [this](Button*) -> bool
	{
		if (selectedTile != NULL && selectedTowerInBuildMenu == NULL && !isOptionBoxVisible())
		{
			game->sell(selectedTile);
			return true;
		}

		return false;
	}));
	inGameButtons.push_back(new Button(getRenderer(), 600, 0, 112, 51, "./gfx/button/ingame-menuf10-112x51.png", [this](Button*) -> bool
	{
		game->getEngine()->pushState((State*)game->inGameMenuState);
		return true;
	}));

	//Optionbox buttons and background
	optionBoxBGx1 = new Sprite(game, "./gfx/menu/popup-menu-1x-40x43.png", 0, 0, 40, 43);
	optionBoxBGx2 = new Sprite(game, "./gfx/menu/popup-menu-2x-67x43.png", 0, 0, 67, 43);
	optionBoxBGx3 = new Sprite(game, "./gfx/menu/popup-menu-3x-94x43.png", 0, 0, 94, 43);
	optionBoxBGx4 = new Sprite(game, "./gfx/menu/popup-menu-4x-122x43.png", 0, 0, 122, 43);
	optionBoxBGx5 = new Sprite(game, "./gfx/menu/popup-menu-5x-149x43.png", 0, 0, 149, 43);
	optionBoxBGx6 = new Sprite(game, "./gfx/menu/popup-menu-6x-177x43.png", 0, 0, 177, 43);

	optionBoxButtonStorage[BUTTON_BASE] = (new Sprite(game, "./gfx/button/menu-button-base-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_BASE]->set_type(BUTTON_BASE);
	optionBoxButtonStorage[BUTTON_BASIC] = (new Sprite(game, "./gfx/button/menu-button-basic-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_BASIC]->set_type(BUTTON_BASIC);
	optionBoxButtonStorage[BUTTON_BOMB] = (new Sprite(game, "./gfx/button/menu-button-bomb-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_BOMB]->set_type(BUTTON_BOMB);
	optionBoxButtonStorage[BUTTON_BOOST] = (new Sprite(game, "./gfx/button/menu-button-boost-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_BOOST]->set_type(BUTTON_BOOST);
	optionBoxButtonStorage[BUTTON_RANGE] = (new Sprite(game, "./gfx/button/menu-button-range-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_RANGE]->set_type(BUTTON_RANGE);
	optionBoxButtonStorage[BUTTON_SELL] = (new Sprite(game, "./gfx/button/menu-button-sell-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_SELL]->set_type(BUTTON_SELL);
	optionBoxButtonStorage[BUTTON_SPEED] = (new Sprite(game, "./gfx/button/menu-button-speed-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_SPEED]->set_type(BUTTON_SPEED);
	optionBoxButtonStorage[BUTTON_UPGRADE] = (new Sprite(game, "./gfx/button/menu-button-upgrade-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_UPGRADE]->set_type(BUTTON_UPGRADE);
	optionBoxButtonStorage[BUTTON_NOUPGRADE] = (new Sprite(game, "./gfx/button/menu-button-noupgrade-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_NOUPGRADE]->set_type(BUTTON_NOUPGRADE);
	optionBoxButtonStorage[BUTTON_WALL] = (new Sprite(game, "./gfx/button/menu-button-wall-30x30.png", 0, 0, 30, 30));
	optionBoxButtonStorage[BUTTON_WALL]->set_type(BUTTON_WALL);

	SFX_cant_build = new Sound("./snd/SFX_1.wav", false, 0);

	sound_btn_repeat_delay = 0.25f;
	sound_btn_repeat_value = sound_btn_repeat_delay;

	map = new Sprite(game, "./gfx/map/map.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	mapGrid = new Sprite(game, "./gfx/map/map-grid.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	mapWall = new Sprite(game, "./gfx/map/map-wall.png", 0, 0, GRIDWIDTH, GRIDHEIGHT);
	mapEntrance = new Sprite(game, "./gfx/map/map-entrance.png", 0, 280, TILESIZE, TILESIZE);
	mapExit = new Sprite(game, "./gfx/map/map-portal.png", 520, 239, TILESIZE, TILESIZE);

	menuBackground = new Sprite(game, "./gfx/menu/menu-bg.png", 580, 0, WWIDTH - MENUWIDTH, WHEIGHT);
	menuMoneyScore = new Sprite(game, "./gfx/menu/menu-money-score-200x90.png", GRIDWIDTH, 50, 200, 90);
	menuBuild = new Sprite(game, "./gfx/menu/menu-build-200x90.png", GRIDWIDTH, 140, 200, 90);
	menuUpgrade = new Sprite(game, "./gfx/menu/menu-upgrade-sell-200x90.png", GRIDWIDTH, 250, 200, 90);
	menuInfo = new Sprite(game, "./gfx/menu/menu-information-200x240.png", GRIDWIDTH, 360, 200, 240);
	menuLives = new Sprite(game, "./gfx/menu/heart_big-25x25.png", 624, 104, 18, 18);

	pressEnterToStartText = new Text(getRenderer(), "Deploy towers, then press enter to start", 70, 550, game->standard_font_32);

	//Green and red rects
	availableTileMarker = new Sprite(game, "./gfx/misc/spot-free-4.png", 0, 0, 40, 40);
	occupiedTileMarker = new Sprite(game, "./gfx/misc/spot-taken-4.png", 0, 0, 40, 40);

	scoreText = new Text(getRenderer(), "", 0, (int)menuMoneyScore->get_y() + 30, game->standard_font_16);
	update_score();
	splitMoneyScore = new Text(getRenderer(), "-------------------------------------", (int)menuMoneyScore->get_x() + 30, (int)menuMoneyScore->get_y() + 45, game->standard_font_12);
	moneyText = new Text(getRenderer(), "", 0, (int)menuMoneyScore->get_y() + 55, game->standard_font_16);
	update_money();
	livesText = new Text(getRenderer(), "", 0, (int)menuMoneyScore->get_y() + 55, game->standard_font_16);
	update_lives();
	levelText = new Text(getRenderer(), "Wave: 1", (int)menuMoneyScore->get_x() + 25, (int)menuMoneyScore->get_y() + 30, game->standard_font_16);
	timerText = new Text(getRenderer(), "", 255, 255, 255, 0, 5, game->standard_font_12);

	fpsTimer.start();
	fpsText = new Text(getRenderer(), "", 255, 255, 255, 5, 5, game->standard_font_12);
	fpsText->hide();

	return true;
}

void GamePlayState::cleanup()
{
	delete speedText;
	delete selectionMarker;

	delete SFX_cant_build;

	delete optionBoxBGx1;
	delete optionBoxBGx2;
	delete optionBoxBGx3;
	delete optionBoxBGx4;
	delete optionBoxBGx5;
	delete optionBoxBGx6;

	delete map;
	delete mapGrid;
	delete mapWall;
	delete mapEntrance;
	delete mapExit;

	delete menuBackground;
	delete menuMoneyScore;
	delete menuBuild;
	delete menuUpgrade;
	delete menuInfo;
	delete menuLives;
	delete pressEnterToStartText;
	delete scoreText;
	delete livesText;
	delete moneyText;
	delete levelText;
	delete timerText;
	delete fpsText;

	delete splitMoneyScore;

	delete availableTileMarker;
	delete occupiedTileMarker;

	//Delete available towers on menu
	for (TowerList::iterator iter_build_obj = buildList.begin(); iter_build_obj != buildList.end(); iter_build_obj++)
	{
		delete (*iter_build_obj);
	}

	//Delete buttons
	for (ButtonList::iterator iter_ingame_button = inGameButtons.begin(); iter_ingame_button != inGameButtons.end(); iter_ingame_button++)
	{
		delete (*iter_ingame_button);
	}

	for (std::map<int, Sprite*>::iterator it = optionBoxButtonStorage.begin(); it != optionBoxButtonStorage.end(); it++)
	{
		delete (*it).second;
	}
}

void GamePlayState::reset()
{
	selectionMarker->set_x(-5);
	selectionMarker->set_y(-5);
	selectionMarker->hide();

	gameStarted = false;
	selectedTowerInBuildMenu = NULL;
	selectedTile = NULL;
	optionBoxVisible = false;

	update_score();
	update_money();
	update_lives();
	update_level();
}

bool GamePlayState::handleEvent(const SDL_Event &ev)
{
	if (ev.key.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_F10)
	{
		game->getEngine()->pushState((State*)game->inGameMenuState);
		return true;
	}

	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);

	if (ev.type == SDL_KEYDOWN)
	{
		if (DEBUGMODE)
		{
			if (gameStarted && ev.key.keysym.sym == SDLK_m)
			{
				Enemy* const e = new Enemy(game, FISH, -40, 7 * TILESIZE + 5, 30, 30, 100);
				game->get_enemies()->push_back(e);
				return true;
			}
			if (ev.key.keysym.sym == SDLK_c)
			{
				for (EnemyList::iterator it = game->get_enemies()->begin(); it != game->get_enemies()->end(); it++)
				{
					delete (*it);
				}
				game->get_enemies()->clear();

				return true;
			}
		}
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			cancelSelection();
			return true;
		}
		else if (ev.key.keysym.sym == SDLK_F11)
		{
			fpsText->is_visible() ? fpsText->hide() : fpsText->show();
			return true;
		}
		else if (ev.key.keysym.sym == SDLK_KP_PLUS || ev.key.keysym.sym == SDLK_PLUS)
		{
			if (game->getEngine()->getGameSpeed() < 2.f)
			{
				game->getEngine()->setGameSpeed(game->getEngine()->getGameSpeed() * 2.f);
				std::string s("x");
				s.append(Utils::ftos(game->getEngine()->getGameSpeed()));
				speedText->update_text(s);
				speedText->set_center_x(585);
				game->getEngine()->getGameSpeed() == 1.f ? speedText->hide() : speedText->show();
			}

			return true;
		}
		else if (ev.key.keysym.sym == SDLK_KP_MINUS || ev.key.keysym.sym == SDLK_MINUS)
		{
			if (game->getEngine()->getGameSpeed() > .5f)
			{
				game->getEngine()->setGameSpeed(game->getEngine()->getGameSpeed() / 2.f);
				std::string s("x");
				s.append(Utils::ftos(game->getEngine()->getGameSpeed()));
				speedText->update_text(s);
				speedText->set_center_x(585);
				game->getEngine()->getGameSpeed() == 1.f ? speedText->hide() : speedText->show();
			}

			return true;
		}
		else if (!DEBUGMODE && gameStarted && ev.key.keysym.sym == SDLK_n)
		{
			game->sendNewWave();

			return true;
		}

		if (selectedTowerInBuildMenu == NULL)
		{
			if (!gameStarted && ev.key.keysym.sym == SDLK_RETURN)
			{
				if (getSubState() == GAME_PAUSED)
				{
					setSubState(GAMEPLAY_RUNNING);
					gameStarted = true;
					game->sendNewWave();
					game->get_grid()->clear_paths();
					for (EnemyList::iterator it = game->get_enemies()->begin(); it != game->get_enemies()->end(); it++)
					{
						(*it)->try_update_path();
					}

					return true;
				}
			}
			else if (ev.key.keysym.sym == SDLK_SPACE)
			{
				if (selectedTile != NULL)
				{
					setOptionBoxVisible(!isOptionBoxVisible());
				}

				return true;
			}

			updateOptionBox();
			int position = 0;
			for (Sprite_List::iterator it = optionBox.begin(); it != optionBox.end(); it++)
			{
				int posX = (int)selectionMarker->get_x() + 2;
				int posY = (int)selectionMarker->get_y() + 2;
				GridPosition pos = GridPosition(0, 0);
				Tile* tile = game->get_grid()->get_tile_from_mouse(posX, posY);
				if (tile != NULL)
					pos = tile->get_position();

				if (ev.key.keysym.sym == SDLK_1 && position == 1)
				{
					if (optionBoxDoSelection((*it), pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_2 && position == 2)
				{
					if (optionBoxDoSelection((*it), pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_3 && position == 3)
				{
					if (optionBoxDoSelection((*it), pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_4 && position == 4)
				{
					if (optionBoxDoSelection((*it), pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_5 && position == 5)
				{
					if (optionBoxDoSelection((*it), pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_u)
				{
					if (optionBoxDoSelection(BUTTON_UPGRADE, pos))
						break;
				}
				else if (ev.key.keysym.sym == SDLK_s)
				{
					if (optionBoxDoSelection(BUTTON_SELL, pos))
						break;
				}
				position++;
			}

			if (Utils::isArrowKey(ev))
			{
				int mark_row = 0;
				int mark_col = 0;
				if (selectedTile == NULL)
				{
					if (lastSelectedTile != NULL)
					{
						mark_row = lastSelectedTile->get_position().first;
						mark_col = lastSelectedTile->get_position().second;
					}
				}
				else if (selectedTile != NULL)
				{
					mark_row = selectedTile->get_position().first;
					mark_col = selectedTile->get_position().second;
				}

				Tile* next_tile = NULL;
				switch (ev.key.keysym.sym)
				{
				case SDLK_LEFT:
					next_tile = game->get_grid()->get_tile(mark_row, mark_col - 1);
					break;
				case SDLK_RIGHT:
					next_tile = game->get_grid()->get_tile(mark_row, mark_col + 1);
					break;
				case SDLK_UP:
					next_tile = game->get_grid()->get_tile(mark_row - 1, mark_col);
					break;
				case SDLK_DOWN:
					next_tile = game->get_grid()->get_tile(mark_row + 1, mark_col);
				}

				if (next_tile != NULL)
				{
					select(next_tile);
				}

			}

			return true;
		}
		else //Player has selected a tower to build
		{
			//Player have an object from menu selected
			if (ev.key.keysym.sym == SDLK_LEFT)
			{
				TowerList::iterator it = buildList.end();
				while (buildList.size() > 1)
				{
					it--;
					if (it == buildList.begin())
						break;
					if ((*it) == selectedTowerInBuildMenu)
					{
						it--;
						selectFromBuildMenu((*it));
						break;
					}
				}

				return true;
			}
			else if (ev.key.keysym.sym == SDLK_RIGHT)
			{
				TowerList::iterator it = buildList.begin();
				while (buildList.size() > 1)
				{
					if ((*it) == selectedTowerInBuildMenu)
					{
						it++;
						if (it == buildList.end())
						{
							break;
						}
						selectFromBuildMenu((*it));
						break;
					}
					it++;
				}

				return true;
			}

			return false;
		}
	}
	else if (ev.type == SDL_MOUSEBUTTONDOWN)
	{
		if (ev.button.button == SDL_BUTTON_LEFT)
		{
			if (m_x <= GRIDWIDTH && m_y <= GRIDHEIGHT)
			{ //Mouse within Grid
				Tile* tile = game->get_grid()->get_tile_from_mouse(m_x, m_y);
				if (isOptionBoxVisible())
				{
					bool done = false;
					Sprite_List::iterator it = optionBox.begin();

					// if mouse is inside OptionBox
					if ((*it)->overlaps(m_x, m_y))
					{
						if (selectedTile != NULL)
						{
							tile = selectedTile;
						}
						for (; it != optionBox.end(); it++)
						{
							if ((*it)->overlaps(m_x, m_y))
							{
								if (tile != NULL)
								{
									done = optionBoxDoSelection((*it), tile->get_position());
								}
							}

							if (done)
							{
								break;
							}
						}
					}

					else //if mouse is outside the box
					{
						if (tile != NULL && tile->get_tower() == NULL)
						{
							cancelSelection();
							setOptionBoxVisible(false);
						}
						else
						{
							//Select Tower on this position
							cancelSelection();
							setOptionBoxVisible(false);
							select(tile);
						}
					}
				}

				else //if OptionBox is hidden

				{
					if (selectedTowerInBuildMenu != NULL && tile->get_tower() == NULL)
					{
						//Create new tower
						game->createNewTower(selectedTowerInBuildMenu->get_type(), tile->get_position(), *game->get_grid());
					}
					else if (selectedTowerInBuildMenu == NULL)
					{
						select(tile);
					}
					else // tower is occupying this tile, cannot build there
					{
						SFX_cant_build->play();
					}
				}
			} //Mouse on grid

			else //Mouse on menu

			{
				if (isOptionBoxVisible())
				{
					setOptionBoxVisible(false);
				}
				//Stuff on menu
				for (ButtonList::iterator it = inGameButtons.begin(); it != inGameButtons.end(); it++)
				{
					if ((*it)->overlaps(m_x, m_y))
					{
						if ((*it)->hasAction())
						{
							return (*it)->performAction();
						}
					}
				}// Iterator-loop ends

				for (TowerList::iterator it = buildList.begin(); it != buildList.end(); it++)
				{
					if ((*it)->overlaps(m_x, m_y))
					{
						selectFromBuildMenu((*it));
					}
				}
			}
		} //Mousebutton left

		else if (ev.button.button == SDL_BUTTON_RIGHT)
		{
			if (selectedTowerInBuildMenu == NULL && selectionMarker->overlaps(m_x, m_y))
			{
				if (selectionMarker->is_visible())
				{
					setOptionBoxVisible(!isOptionBoxVisible());
				}
			}
			else
			{
				if (isOptionBoxVisible())
				{
					setOptionBoxVisible(false);
				}
				else
				{
					cancelSelection();
				}
			}
		}
	}
	else if (ev.type == SDL_MOUSEMOTION)
	{
		if (selectedTile == NULL && selectedTowerInBuildMenu == NULL)
		{
			//Loop through build objects
			for (TowerList::iterator iter_build_obj = buildList.begin(); iter_build_obj != buildList.end(); iter_build_obj++)
			{
				if ((*iter_build_obj)->overlaps(m_x, m_y))
				{
					hoveredTowerInBuildMenu = (*iter_build_obj);
					return true;
				}
			}
		}
		hoveredTowerInBuildMenu = NULL;
	}

	return false;
}

void GamePlayState::cancelSelection()
{
	updateOptionBox();
	if (selectedTile != NULL && selectedTile->get_tower() != NULL)
		selectedTile->get_tower()->set_selected(false);
	if (selectedTowerInBuildMenu != NULL)
	{
		selectedTowerInBuildMenu->set_selected(false);
	}
	selectedTile = NULL;
	selectedTowerInBuildMenu = NULL;
	selectionMarker->hide();
	setOptionBoxVisible(false);
}

void GamePlayState::setOptionBoxVisible(const bool visible)
{
	if (visible)
	{
		if (selectionMarker->is_visible())
		{
			optionBoxVisible = true;
			updateOptionBox();
		}
	}
	else
	{
		optionBoxVisible = false;
	}
}

const bool GamePlayState::isOptionBoxVisible() const
{
	return optionBoxVisible;
}

void GamePlayState::updateOptionBox()
{
	float button_spacing = 27.f;
	int button_offset_x = 6;
	int button_offset_y = 6;

	optionBox.clear();
	int optbox_pos_x;
	int optbox_pos_y;

	if (selectedTile == NULL || selectedTile->get_tower() == NULL)
	{
		optionBox.push_back(optionBoxBGx3);
		if (selectionMarker->is_visible())
		{
			composeOptionBoxPosition(optbox_pos_x, optbox_pos_y, optionBoxBGx3);
			float button_x = (float)(optbox_pos_x + button_offset_x);
			float button_y = (float)(optbox_pos_y + button_offset_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_BASE]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_BOOST]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_WALL]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);
		}
	}

	else if (selectedTile != NULL && selectedTile->get_tower() != NULL)
	{
		BaseTower* t = selectedTile->get_tower();
		if (t->get_type() == towers::SIMPLE)
		{
			optionBox.push_back(optionBoxBGx5);
			composeOptionBoxPosition(optbox_pos_x, optbox_pos_y, optionBoxBGx5);
			float button_x = (float)(optbox_pos_x + button_offset_x);
			float button_y = (float)(optbox_pos_y + button_offset_y);

			//Push back the 4 upgrade-buttons + sell-button, offset px apart from each other
			optionBox.push_back(optionBoxButtonStorage[BUTTON_BASIC]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_SPEED]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_RANGE]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_BOMB]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_SELL]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);
		}
		else if (t->get_level() == t->get_max_level())
		{
			optionBox.push_back(optionBoxBGx2);
			composeOptionBoxPosition(optbox_pos_x, optbox_pos_y, optionBoxBGx2);
			float button_x = (float)(optbox_pos_x + button_offset_x);
			float button_y = (float)(optbox_pos_y + button_offset_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_NOUPGRADE]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_SELL]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);
		}
		else if (t->get_type() == towers::WALL)
		{
			optionBox.push_back(optionBoxBGx1);
			composeOptionBoxPosition(optbox_pos_x, optbox_pos_y, optionBoxBGx1);
			float button_x = (float)(optbox_pos_x + button_offset_x);
			float button_y = (float)(optbox_pos_y + button_offset_y);

			//Push back a Sell-button
			optionBox.push_back(optionBoxButtonStorage[BUTTON_SELL]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);
		}
		else
		{
			optionBox.push_back(optionBoxBGx2);
			composeOptionBoxPosition(optbox_pos_x, optbox_pos_y, optionBoxBGx2);
			float button_x = (float)(optbox_pos_x + button_offset_x);
			float button_y = (float)(optbox_pos_y + button_offset_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_UPGRADE]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);

			optionBox.push_back(optionBoxButtonStorage[BUTTON_SELL]);
			optionBox.back()->set_x(button_x);
			button_x += button_spacing;
			optionBox.back()->set_y(button_y);
		}
	}
}

void GamePlayState::composeOptionBoxPosition(int &optbox_pos_x, int &optbox_pos_y, Sprite* const optionbox)
{
	int box_offset_x = 25;
	int box_offset_y = -25;
	if (selectedTile == NULL)
	{
		optbox_pos_x = (int)selectionMarker->get_x() + box_offset_x;
		optbox_pos_y = (int)selectionMarker->get_y() + box_offset_y;
	}
	else
	{
		optbox_pos_x = selectedTile->get_x_pixel_pos() + box_offset_x;
		optbox_pos_y = selectedTile->get_y_pixel_pos() + box_offset_y;
	}

	if ((optbox_pos_x + optionbox->get_width()) > GRIDWIDTH)
		optbox_pos_x -= (optbox_pos_x + optionbox->get_width()) - GRIDWIDTH;

	if ((optbox_pos_y + optionbox->get_height()) > WHEIGHT)
		optbox_pos_y -= (TILESIZE + optionbox->get_height());

	if (optbox_pos_x < 0)
		optbox_pos_x = 0;

	if (optbox_pos_y < 0)
		optbox_pos_y = -box_offset_y;

	optionbox->set_x((float)optbox_pos_x);
	optionbox->set_y((float)optbox_pos_y);
}

bool GamePlayState::optionBoxDoSelection(Sprite* const curr_op_sel, const GridPosition &position)
{
	return optionBoxDoSelection(curr_op_sel->get_int_type(), position);
}

bool GamePlayState::optionBoxDoSelection(const int type, const GridPosition &position)
{
	BaseTower* const t = game->get_grid()->get_tile(position)->get_tower();
	/**
	 * Selects an option on the optionbox
	 */
	switch (type)
	{
	case BUTTON_BASE:
	{
		if (t == NULL)
		{
			game->createNewTower(towers::SIMPLE, position, *game->get_grid());
			return true;
		}
		break;
	}
	case BUTTON_BOOST:
	{
		if (game->get_grid()->get_tile(position)->get_tower() == NULL)
		{
			game->createNewTower(towers::BOOST, position, *game->get_grid());
			return true;
		}
		break;
	}
	case BUTTON_BASIC:
	{
		game->upgrade_tower(t, towers::BASIC);
		return true;
	}
	case BUTTON_BOMB:
	{
		game->upgrade_tower(t, towers::BOMB);
		return true;
	}
	case BUTTON_RANGE:
	{
		game->upgrade_tower(t, towers::RANGE);
		return true;
	}
	case BUTTON_SELL:
	{
		game->sell(selectedTile);
		return true;
	}
	case BUTTON_SPEED:
	{
		game->upgrade_tower(t, towers::SPEED);
		return true;
	}
	case BUTTON_UPGRADE:
	{
		if (t != NULL)
		{
			game->upgrade_tower(t);
			return true;
		}
		else
		{
			return false;
		}
		break;
	}
	case BUTTON_WALL:
	{
		game->createNewTower(towers::WALL, position, *game->get_grid());
		return true;
	}
	default:
		break;
	} //Switch
	return false;
}

void GamePlayState::select(Tile* const tile)
{
	BaseTower* tower = NULL;
	if (selectedTile != NULL)
	{
		tower = selectedTile->get_tower();
	}

	if (selectedTowerInBuildMenu != NULL)
	{
		selectedTowerInBuildMenu->set_selected(false);
		selectedTowerInBuildMenu = NULL;
	}
	selectedTile = tile;
	lastSelectedTile = selectedTile;
	if (tower != NULL)
	{
		tower->set_selected(false);
	}

	tower = selectedTile->get_tower();

	selectionMarker->show();
	selectionMarker->set_x(selectedTile->get_x_pixel_pos() - 2.f);
	selectionMarker->set_y(selectedTile->get_y_pixel_pos() - 2.f);
	if (tower != NULL)
	{
		tower->set_selected(true);
	}
	updateOptionBox();
}

void GamePlayState::selectFromBuildMenu(BaseTower* const tower)
{
	if (selectedTile != NULL && selectedTile->get_tower() != NULL)
	{
		selectedTile->get_tower()->set_selected(false);
		selectedTile = NULL;
	}

	selectedTowerInBuildMenu = tower;
	selectionMarker->show();
	selectionMarker->set_x(selectedTowerInBuildMenu->get_x() - 2);
	selectionMarker->set_y(selectedTowerInBuildMenu->get_y() - 2);
	tower->set_selected(true);
	updateOptionBox();
}


void GamePlayState::update(const float &timeStep)
{
	int mx, my;
	Uint8 mousestate = SDL_GetMouseState(&mx, &my);
	if (mousestate & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if (soundButton->overlaps(mx, my))
		{
			sound_btn_repeat_value -= timeStep; // TODO redo this
			if (sound_btn_repeat_value <= 0)
			{
				Button* const btn = soundButton;
				if (mx >= btn->get_x() + 26 && mx <= btn->get_x() + 39)
				{
					if (my >= btn->get_y() + 6 && my <= btn->get_y() + 19)
					{
						// this is the plus sign, increase volume
						bool wasDisabled = !Sound::enabled();
						Sound::set_volume(Sound::get_volume() + 1);
						if (wasDisabled)
						{
							LOG_DEBUG << "Turning on music";
							game->setMusicPlaying(true);
						}
						else
						{
							LOG_DEBUG << "New music volume: " << Sound::get_volume();
						}

						sound_btn_repeat_value = sound_btn_repeat_delay;
					}
					else if (my >= btn->get_y() + 22 && my <= btn->get_y() + 35)
					{
						// this is the minus sign, decrease volume
						if (Sound::get_volume() > 0)
						{
							if (Sound::set_volume(Sound::get_volume() - 1) == 0)
							{
								LOG_DEBUG << "Turning off music";
								game->setMusicPlaying(false);
							}
							else
							{
								LOG_DEBUG << "New music volume: " << Sound::get_volume();
							}
						}

						sound_btn_repeat_value = sound_btn_repeat_delay;
					}
				}
			}
		}
	}
	else
	{
		sound_btn_repeat_value = 0;
	}
	game->update(timeStep);

	update_score();
	update_money();
	update_timer();
	update_lives();
	update_level();
	updateFpsText();

}

void GamePlayState::update_lives()
{
	std::string livesStr = Utils::itos(game->getLives());
	if (livesStr != livesText->getSourceText())
	{
		livesText->update_text(livesStr);
		livesText->set_x(menuMoneyScore->get_x() + 50);
	}
}

void GamePlayState::update_money()
{
	std::string moneyStr = "$" + Utils::itos(game->getMoney());
	if (moneyStr != moneyText->getSourceText())
	{
		moneyText->update_text(moneyStr);
		moneyText->set_x(menuMoneyScore->get_x() + (180 - moneyText->get_width()));
	}
}

void GamePlayState::update_score()
{
	std::string scoreStr = Utils::itos(game->getScore());
	if (scoreStr != scoreText->getSourceText())
	{
		scoreText->update_text(scoreStr);
		scoreText->set_x(menuMoneyScore->get_x() + (180 - scoreText->get_width()));
	}
}
void GamePlayState::update_level()
{
	std::string levelStr = "Wave: " + Utils::itos(game->level_control->get_level());
	if (levelStr != levelText->getSourceText())
	{
		levelText->update_text(levelStr);
	}
}
void GamePlayState::update_timer()
{
	std::string timerStr;
	if (game->level_control->time_before_next_wave() == -1)
	{
		timerStr = "Press N for next wave";
	}
	else
	{
		int timer = game->level_control->time_before_next_wave();
		timerStr = "Next wave in: " + Utils::itos(timer);
	}

	if (timerStr != timerText->getSourceText())
	{
		timerText->update_text(timerStr);
		timerText->set_x((float)(590 - timerText->get_width()));
	}
}

void GamePlayState::updateFpsText()
{
	if (fpsTimer.get_ticks() > 1000)
	{
		fpsTimer.start();
		const int currentFps = game->getEngine()->getFPS();
		std::string tmp = "FPS: ";
		tmp.append(Utils::itos(currentFps));
		fpsText->update_text(tmp);
	}
}

void GamePlayState::render(SDL_Renderer* const renderer)
{
	map->draw(renderer);
	if (gridVisible)
	{
		mapGrid->draw(renderer);
	}

	mapWall->draw(renderer);
	mapEntrance->draw(renderer);

	//Draw visual connections between boostertowers and other towers
	for (TowerList::iterator it = game->get_towers()->begin(); it != game->get_towers()->end(); it++)
	{
		if ((*it)->get_type() == towers::BOOST)
		{
			for (TowerList::iterator it2 = game->get_towers()->begin(); it2 != game->get_towers()->end(); it2++)
			{
				if ((*it)->get_type() != towers::WALL)
				{
					if ((*it)->get_distance_to((*it2)) <= (*it)->get_range_in_pixels())
					{
						int line_strength = (*it)->get_level();
						line_strength = 0x33 + line_strength * 12;

						Sint16 x1, x2, y1, y2;
						x1 = (Sint16)((*it)->get_x() + TILESIZE / 2.0f);
						y1 = (Sint16)((*it)->get_y() + TILESIZE / 2.0f);
						x2 = (Sint16)((*it2)->get_x() + TILESIZE / 2.0f);
						y2 = (Sint16)((*it2)->get_y() + TILESIZE / 2.0f);
						lineRGBA(renderer, x1, y1, x2, y2, 0xFF, 0xFF, 0, line_strength);
					}
				}
			}
		}
	}

	for (TowerList::iterator it = game->get_towers()->begin(); it != game->get_towers()->end(); it++)
	{
		(*it)->draw(renderer);
	}

	menuBackground->draw(renderer);
	menuMoneyScore->draw(renderer);
	menuBuild->draw(renderer);
	menuUpgrade->draw(renderer);
	menuInfo->draw(renderer);
	menuLives->draw(renderer);

	moneyText->draw(renderer);
	splitMoneyScore->draw(renderer);
	scoreText->draw(renderer);
	livesText->draw(renderer);
	levelText->draw(renderer);
	timerText->draw(renderer);
	speedText->draw(renderer);

	for (EnemyList::iterator it = game->get_enemies()->begin(); it != game->get_enemies()->end(); it++)
	{
		(*it)->draw(renderer);
	}

	for (EnemyList::iterator it = game->get_enemies()->begin(); it != game->get_enemies()->end(); it++)
	{
		(*it)->draw_health_bar(renderer);
	}

	selectionMarker->draw(renderer);

	BaseTower* t = NULL;
	if (selectedTile != NULL && selectedTile->get_tower() != NULL)
	{
		t = selectedTile->get_tower();
		t->drawRange(renderer);
	}
	else if (hoveredTowerInBuildMenu != NULL)
	{
		t = hoveredTowerInBuildMenu;
	}
	else if (selectedTowerInBuildMenu != NULL)
	{
		t = selectedTowerInBuildMenu;
	}

	if (t != NULL)
	{
		Sprite_List* s = t->get_infosprites();
		for (Sprite_List::iterator it = s->begin(); it != s->end(); it++)
		{
			(*it)->draw(renderer);
		}
	}

	mapExit->draw(renderer);

	for (ProjectileList::iterator it = game->get_projectiles()->begin(); it != game->get_projectiles()->end(); it++)
	{
		(*it)->draw(renderer);
	}

	for (TowerList::iterator it = buildList.begin(); it != buildList.end(); it++)
	{
		(*it)->draw(renderer);
	}

	for (ButtonList::iterator it = inGameButtons.begin(); it != inGameButtons.end(); it++)
	{
		(*it)->draw(renderer);
	}

	if (isOptionBoxVisible())
	{
		for (Sprite_List::iterator it = optionBox.begin(); it != optionBox.end(); it++)
		{
			(*it)->draw(renderer);
		}
	}

	if (selectedTowerInBuildMenu != NULL)
	{
		int m_x, m_y;
		SDL_GetMouseState(&m_x, &m_y);
		int pos_x = m_x;
		int pos_y = m_y;
		Tile* hovered_tile = game->get_grid()->get_tile_from_mouse(m_x, m_y);

		if (m_x < GRIDWIDTH && hovered_tile != NULL)
		{
			pos_x = hovered_tile->get_x_pixel_pos();
			pos_y = hovered_tile->get_y_pixel_pos();
			if (hovered_tile->get_tower() == NULL)
			{
				//There is no tower, draw green square
				selectedTowerInBuildMenu->draw(renderer, pos_x, pos_y);
				selectedTowerInBuildMenu->drawRange(renderer, (float)pos_x, (float)pos_y);
				if (game->getMoney() >= selectedTowerInBuildMenu->get_cost_buy())
				{
					availableTileMarker->draw(renderer, pos_x, pos_y);
				}
				else
				{
					occupiedTileMarker->draw(renderer, pos_x, pos_y);
				}
			}
			else if (hovered_tile->get_tower() != NULL)
			{
				//There is tower here, draw red square
				//Don't draw tower if there is tower of same type
				if (selectedTowerInBuildMenu->get_type() != hovered_tile->get_tower()->get_type())
				{
					selectedTowerInBuildMenu->draw(renderer, pos_x, pos_y);
				}
				selectedTowerInBuildMenu->drawRange(renderer, (float)pos_x, (float)pos_y);
				occupiedTileMarker->draw(renderer, pos_x, pos_y);
			}
		}
		else
		{
			selectedTowerInBuildMenu->draw(renderer, m_x - 20, m_y - 20);
		}
	}

	if (!gameStarted) pressEnterToStartText->draw(renderer);
	fpsText->draw(renderer);
}

