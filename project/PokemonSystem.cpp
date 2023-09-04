/***********************************************************************
 * File: PokemonSystem.h
 * Author: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Create Date: 2023/05/30
 * Editor: Kelvin Tan Chen Hao, JHENG YUAN CHEN, YU JIE YUAN
 * Update Date: 2023/06/14
 * Description: This program is for PokemonSystem class cpp
***********************************************************************/
#include "PokemonSystem.h"

// Intent:	PokemonSystem constructor
// Pre:		None
// Post:	None
PokemonSystem::PokemonSystem()
{
    // link to the webpage of database
    listener = new http_listener(U("http://localhost:8080"));
}

// Intent:  receive actions from web pages and execute
// Pre:		None
// Post:	None
void PokemonSystem::exe()
{
    // setup a listener for GET requests
    listener->support(methods::GET, [&](http_request request) {
        request.extract_json().then([&, request](json::value body) {

            // set the response
            http_response response;
            response.set_status_code(status_codes::OK);
            response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            response.headers().add(U("Access-Control-Allow-Methods"), U("*"));
            response.headers().add(U("Content-Type"), U("application/json"));

            // cut the URL and take out related instructions
            auto query_params = uri::split_query(request.request_uri().query());

            // find the "readFile" from URL
            if (query_params.find(U("readFile")) != query_params.end())
            {
                // take the values from URL, and transfer to string
                auto text1 = to_utf8string(uri::decode(query_params[U("testCase")]));
                auto text2 = to_utf8string(uri::decode(query_params[U("pokemon")]));
                auto text3 = to_utf8string(uri::decode(query_params[U("move")]));
                auto text4 = to_utf8string(uri::decode(query_params[U("gameData")]));

                // set a battle
                player = new Player();
                oponent = new Player();
                battle = new BattleManager(player, oponent);

                pokemonDatas.clear();
                outputFile.close();
                readFile(text1, text2, text3, text4); // call readFile()

                // write json value
                json::value command;
                command[U("Command")] = json::value("ReadData");
                response.set_body(command); // set json value to response buffer

                // combine jsons of the pokemon data together
                json::value pokemonDataArr;

                for (int i = 0; i < pokemonDatas.size(); i++)
                {
                    pokemonDataArr[i] = pokemonDatas[i];
                }

                response.set_body(pokemonDataArr); // set json value to response buffer

            }
            else if (query_params.find(U("move")) != query_params.end())  // find the "move" from URL
            {
                // get player pokemon and oponent pokemon
                Pokemon* sPokemon = player->getPokemons()[player->getCurrentPokemon()];
                Pokemon* ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];
                auto sMoveName = to_utf8string(uri::decode(query_params[U("move")])); // get player move from URL

                // choose a oponent move randomly
                int eMoveNum = ePokemon->getMoves().size();
                int index = rand() % eMoveNum;
                int eCurrentPokemon = oponent->getCurrentPokemon();
                string eMoveName = (ePokemon->getMoves()[index]->getName());
                string whichWin = "";

                // call move() of balttle manager
                battle->move(sMoveName, eMoveName);

                // write json of game datas
                json::value info;
                info[U("Info")] = json::value(wstring(battle->getMessage().begin(), battle->getMessage().end()));
                info[U("IsSuccess")] = json::value(true);
                info[U("sHp")] = json::value(sPokemon->getCurrentHp());
                info[U("eHp")] = json::value(ePokemon->getCurrentHp());

                for (int i = 0; i < sPokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("sPAR")] = json::value(sPokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("sBRN")] = json::value(sPokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("sPSN")] = json::value(sPokemon->getStatus()[PSN]);
                        break;
                    }
                }

                for (int i = 0; i < ePokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("ePAR")] = json::value(ePokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("eBRN")] = json::value(ePokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("ePSN")] = json::value(ePokemon->getStatus()[PSN]);
                        break;
                    }
                }

                if (sPokemon->getCurrentHp() <= 0)
                {
                    string state = "faited";
                    info[U("sState")] = json::value(wstring(state.begin(), state.end()));
                }

                // if oponent already switch pokemon
                if (eCurrentPokemon != oponent->getCurrentPokemon())
                {
                    ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];
                    string state = "faited";
                    info[U("eState")] = json::value(wstring(state.begin(), state.end()));
                    info[U("Name")] = json::value(wstring(ePokemon->getName().begin(), ePokemon->getName().end()));
                    info[U("Hp")] = json::value(ePokemon->getMaxHp());
                    info[U("Atk")] = json::value(ePokemon->getAtk());
                    info[U("Def")] = json::value(ePokemon->getDef());
                    info[U("SpAtk")] = json::value(ePokemon->getSpAtk());
                    info[U("SpDef")] = json::value(ePokemon->getSpDef());
                    info[U("Speed")] = json::value(ePokemon->getSpeed());
                    string type = gLib->changeToTypeName(ePokemon->getType()[0]);
                    info[U("Type1")] = json::value(wstring(type.begin(), type.end()));

                    if (ePokemon->getType().size() >= 2)
                    {
                        type = gLib->changeToTypeName(ePokemon->getType()[1]);
                        info[U("Type2")] = json::value(wstring(type.begin(), type.end()));
                    }
                }

                // write game state
                switch (battle->getGameState())
                {
                case 0:
                    info[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
                    break;
                case 1:
                    whichWin = "sWin"; // player win
                    info[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
                    break;
                case 2:
                    whichWin = "eWin"; // oponent win
                    info[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
                    break;
                }
                response.set_body(info); // set json value to response buffer
                battle->clearMessage(); // clear massage
            }
            else if (query_params.find(U("swap")) != query_params.end()) // find the "swap" from URL
            {
                // get oponent pokemon
                Pokemon* ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];

                // choose a oponent move randomly
                int eMoveNum = ePokemon->getMoves().size();
                int index = rand() % eMoveNum;
                string eMoveName = ePokemon->getMoves()[index]->getName();
                int eCurrentPokemon = oponent->getCurrentPokemon();

                // swap the pokemon
                for (int i = 0; i < player->getPokemons().size(); i++)
                {
                    if (player->getPokemons()[i]->getName() == to_utf8string(uri::decode(query_params[U("swap")]))) 
                    {
                        battle->swap(i, eMoveName);
                    }
                }

                // get player pokemon
                Pokemon* sPokemon = player->getPokemons()[player->getCurrentPokemon()];

                // write json of game datas
                json::value info;
                info[U("Info")] = json::value(wstring(battle->getMessage().begin(), battle->getMessage().end()));
                info[U("sHp")] = json::value(sPokemon->getCurrentHp());
                info[U("eHp")] = json::value(ePokemon->getCurrentHp());

                for (int i = 0; i < sPokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("sPAR")] = json::value(sPokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("sBRN")] = json::value(sPokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("sPSN")] = json::value(sPokemon->getStatus()[PSN]);
                        break;
                    }
                }

                for (int i = 0; i < ePokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("ePAR")] = json::value(ePokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("eBRN")] = json::value(ePokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("ePSN")] = json::value(ePokemon->getStatus()[PSN]);
                        break;
                    }
                }

                if (sPokemon->getCurrentHp() <= 0)
                {
                    string state = "faited";
                    info[U("sState")] = json::value(wstring(state.begin(), state.end()));
                }

                // if oponent already switch pokemon
                if (eCurrentPokemon != oponent->getCurrentPokemon())
                {
                    ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];
                    string state = "faited";
                    info[U("eState")] = json::value(wstring(state.begin(), state.end()));
                    info[U("Name")] = json::value(wstring(ePokemon->getName().begin(), ePokemon->getName().end()));
                    info[U("Hp")] = json::value(ePokemon->getMaxHp());
                    info[U("Atk")] = json::value(ePokemon->getAtk());
                    info[U("Def")] = json::value(ePokemon->getDef());
                    info[U("SpAtk")] = json::value(ePokemon->getSpAtk());
                    info[U("SpDef")] = json::value(ePokemon->getSpDef());
                    info[U("Speed")] = json::value(ePokemon->getSpeed());
                    string type = gLib->changeToTypeName(ePokemon->getType()[0]);
                    info[U("Type1")] = json::value(wstring(type.begin(), type.end()));

                    if (ePokemon->getType().size() >= 2)
                    {
                        type = gLib->changeToTypeName(ePokemon->getType()[1]);
                        info[U("Type2")] = json::value(wstring(type.begin(), type.end()));
                    }
                }

                response.set_body(info); // set json value to response buffer
                battle->clearMessage(); // clear massage
            }
            else if (query_params.find(U("potion")) != query_params.end()) // find the "potion" from URL
            {
                // get player pokemon and oponent pokemon
                Pokemon* sPokemon = player->getPokemons()[player->getCurrentPokemon()];
                Pokemon* ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];
                auto potionName = to_utf8string(uri::decode(query_params[U("potion")])); // get potion name from URL

                // choose a oponent move randomly
                int eMoveNum = ePokemon->getMoves().size();
                int index = rand() % eMoveNum;
                string eMoveName = ePokemon->getMoves()[index]->getName();
                int eCurrentPokemon = oponent->getCurrentPokemon();

                // player use potion
                battle->usePotion(potionName, *sPokemon, eMoveName);

                // write json of game datas
                json::value info;
                info[U("Info")] = json::value(wstring(battle->getMessage().begin(), battle->getMessage().end()));
                info[U("sHp")] = json::value(sPokemon->getCurrentHp());
                info[U("eHp")] = json::value(ePokemon->getCurrentHp());

                if (player->getPokemons()[player->getCurrentPokemon()]->getCurrentHp() <= 0)
                {
                    string state = "faited";
                    info[U("sState")] = json::value(wstring(state.begin(), state.end()));
                }

                for (int i = 0; i < sPokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("sPAR")] = json::value(sPokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("sBRN")] = json::value(sPokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("sPSN")] = json::value(sPokemon->getStatus()[PSN]);
                        break;
                    }
                }

                for (int i = 0; i < ePokemon->getStatus().size(); i++)
                {
                    switch (i)
                    {
                    case PAR:
                        info[U("ePAR")] = json::value(ePokemon->getStatus()[PAR]);
                        break;
                    case BUR:
                        info[U("eBRN")] = json::value(ePokemon->getStatus()[BUR]);
                        break;
                    case PSN:
                        info[U("ePSN")] = json::value(ePokemon->getStatus()[PSN]);
                        break;
                    }
                }

                // if oponent already switch pokemon
                if (eCurrentPokemon != oponent->getCurrentPokemon())
                {
                    ePokemon = oponent->getPokemons()[oponent->getCurrentPokemon()];
                    string state = "faited";
                    info[U("eState")] = json::value(wstring(state.begin(), state.end()));
                    info[U("Name")] = json::value(wstring(ePokemon->getName().begin(), ePokemon->getName().end()));
                    info[U("Hp")] = json::value(ePokemon->getMaxHp());
                    info[U("Atk")] = json::value(ePokemon->getAtk());
                    info[U("Def")] = json::value(ePokemon->getDef());
                    info[U("SpAtk")] = json::value(ePokemon->getSpAtk());
                    info[U("SpDef")] = json::value(ePokemon->getSpDef());
                    info[U("Speed")] = json::value(ePokemon->getSpeed());
                    string type = gLib->changeToTypeName(ePokemon->getType()[0]);
                    info[U("Type1")] = json::value(wstring(type.begin(), type.end()));

                    if (ePokemon->getType().size() >= 2)
                    {
                        type = gLib->changeToTypeName(ePokemon->getType()[1]);
                        info[U("Type2")] = json::value(wstring(type.begin(), type.end()));
                    }
                }

                response.set_body(info); // set json value to response buffer
                battle->clearMessage();  // clear massage
            }
            else if (query_params.find(U("faited")) != query_params.end()) // find the "faited" from URL
            {
                // player switch the pokemon 
                for (int i = 0; i < player->getPokemons().size(); i++)
                {
                    if (player->getPokemons()[i]->getName() == to_utf8string(uri::decode(query_params[U("faited")])))
                    {
                        battle->faited(i);
                    }
                }

                // write json of game datas
                json::value info;
                info[U("Info")] = json::value(wstring(battle->getMessage().begin(), battle->getMessage().end()));
                info[U("sHp")] = json::value(player->getPokemons()[player->getCurrentPokemon()]->getCurrentHp());
                info[U("eHp")] = json::value(oponent->getPokemons()[oponent->getCurrentPokemon()]->getCurrentHp());
                response.set_body(info); // set json value to response buffer
                battle->clearMessage(); // clear massage
            }

            request.reply(response); // return response buffer
        });
    });

    listener->open().wait();

    while (true); // keep program going
}

// Intent:  read files
// Pre:		test case file name, pokemon file name, move file name, game data file name
// Post:	None
void PokemonSystem::readFile(string testCaseFileName, string pokemonFileName, string moveFileNmae, string gameDataFileName)
{
    gLib = new GameLibrary(pokemonFileName, moveFileNmae); // create a game library
    ifstream file(gameDataFileName); // open game data file

    if (file.is_open())
    {
        // read datas of player
        int pokemonNum = 0;
        file >> pokemonNum;

        // write a json of battle datas
        pokemonDatas.push_back(json::value());
        pokemonDatas.back()[U("PokemonNum")] = json::value(pokemonNum);
        pokemonDatas.back()[U("sCurrentPokemon")] = json::value(player->getCurrentPokemon());

        for (int i = 0; i < pokemonNum; i++)
        {
            // read pokemon data fron file 
            string pokemonName = "";
            int moveNum = 0;
            file >> pokemonName >> moveNum;
            vector<string> moveNames(moveNum, "");

            for (int j = 0; j < moveNum; j++) file >> moveNames[j];

            player->addPokemon(*gLib, pokemonName, moveNames); // add a pokemon to player

            // write json of player pokemon and moves
            pokemonDatas.push_back(json::value());
            Pokemon pokemon = gLib->getPokemon(pokemonName);
            pokemonDatas.back()[0][U("Name")] = json::value(wstring(pokemonName.begin(), pokemonName.end()));
            pokemonDatas.back()[0][U("MoveNum")] = json::value(moveNum);
            pokemonDatas.back()[0][U("MaxHp")] = json::value(pokemon.getMaxHp());
            pokemonDatas.back()[0][U("CurrentHp")] = json::value(pokemon.getCurrentHp());
            pokemonDatas.back()[0][U("Atk")] = json::value(pokemon.getAtk());
            pokemonDatas.back()[0][U("Def")] = json::value(pokemon.getDef());
            pokemonDatas.back()[0][U("SpAtk")] = json::value(pokemon.getSpAtk());
            pokemonDatas.back()[0][U("SpDef")] = json::value(pokemon.getSpDef());
            pokemonDatas.back()[0][U("Speed")] = json::value(pokemon.getSpeed());
            string type = gLib->changeToTypeName(pokemon.getType()[0]);
            pokemonDatas.back()[0][U("Type1")] = json::value(wstring(type.begin(), type.end()));

            if (pokemon.getType().size() >= 2)
            {
                type = gLib->changeToTypeName(pokemon.getType()[1]);
                pokemonDatas.back()[0][U("Type2")] = json::value(wstring(type.begin(), type.end()));
            }

            // write json of pokemon moves datas
            for (int j = 1; j <= moveNum; j++)
            {
                json::value moveDatas;
                Move move = gLib->getMove(moveNames[j - 1]);
                moveDatas[U("MoveName")] = json::value(wstring(move.getName().begin(), move.getName().end()));
                string type = gLib->changeToTypeName(move.getType());
                moveDatas[U("MoveType")] = json::value(wstring(type.begin(), type.end()));
                moveDatas[U("MovePower")] = json::value(move.getPower());
                moveDatas[U("MoveAccuracy")] = json::value(move.getAccuracy());
                moveDatas[U("MoveCurrentPP")] = json::value(move.getPp());
                moveDatas[U("MoveMaxPP")] = json::value(move.getPp());
                string effect = gLib->changeToEffectName(move.getAdditionEffect());
                moveDatas[U("MoveEffect")] = json::value(wstring(effect.begin(), effect.end()));

                if (move.getPower() <= 0) moveDatas[U("MoveCategory")] = json::value(L"State");
                else if (move.getSpecialAttack()) moveDatas[U("MoveCategory")] = json::value(L"Special");
                else moveDatas[U("MoveCategory")] = json::value(L"Physical");

                pokemonDatas.back()[j] = moveDatas;
            }
        }

        // read datas of oponent
        pokemonNum = 0;
        file >> pokemonNum;

        for (int i = 0; i < pokemonNum; i++)
        {
            // read pokemon data fron file 
            string pokemonName = "";
            int moveNum = 0;

            file >> pokemonName >> moveNum;
            vector<string> moveNames(moveNum, "");

            for (int j = 0; j < moveNum; j++) file >> moveNames[j];

            oponent->addPokemon(*gLib, pokemonName, moveNames); // add a pokemon to  oponent

            // write oponent current pokemon data
            if (i == oponent->getCurrentPokemon())
            {
                // write json of oponent first pokemon
                pokemonDatas.push_back(json::value());
                Pokemon pokemon = gLib->getPokemon(pokemonName);
                pokemonDatas.back()[0][U("Name")] = json::value(wstring(pokemonName.begin(), pokemonName.end()));
                pokemonDatas.back()[0][U("MaxHp")] = json::value(pokemon.getMaxHp());
                pokemonDatas.back()[0][U("CurrentHp")] = json::value(pokemon.getCurrentHp());
                pokemonDatas.back()[0][U("Atk")] = json::value(pokemon.getAtk());
                pokemonDatas.back()[0][U("Def")] = json::value(pokemon.getDef());
                pokemonDatas.back()[0][U("SpAtk")] = json::value(pokemon.getSpAtk());
                pokemonDatas.back()[0][U("SpDef")] = json::value(pokemon.getSpDef());
                pokemonDatas.back()[0][U("Speed")] = json::value(pokemon.getSpeed());
                string type = gLib->changeToTypeName(pokemon.getType()[0]);
                pokemonDatas.back()[0][U("Type1")] = json::value(wstring(type.begin(), type.end()));

                if (pokemon.getType().size() >= 2)
                {
                    type = gLib->changeToTypeName(pokemon.getType()[1]);
                    pokemonDatas.back()[0][U("Type2")] = json::value(wstring(type.begin(), type.end()));
                }
            }
        }
    }

    if (testCaseFileName != "") readCase(testCaseFileName); // call readCase() to read test case file
}

// Intent:  read and executive test case file
// Pre:		test case file name
// Post:	None
void PokemonSystem::readCase(const string& caseFileName)
{
    // create a output file
    string outputFileName = caseFileName;
    outputFileName.erase(outputFileName.end() - 4, outputFileName.end());
    outputFileName += "_output.txt";
    outputFile.open(outputFileName);

    cout.rdbuf(outputFile.rdbuf()); // set cout to output file

    fstream caseData(caseFileName); // open test case file

    if (!caseData.is_open())
    {
        // error message
        return;
    }

    // get pokemon file name, move file name, game data file name
    string pokemonLib, moveLib, gameData;
    getline(caseData, pokemonLib);
    getline(caseData, moveLib);
    getline(caseData, gameData);

    // create a game library
    GameLibrary* gameLib = new GameLibrary("./data/" + pokemonLib, "./data/" + moveLib);

    fstream gameDataData("./data/" + gameData); // open game data file

    if (!gameDataData.is_open())
    {
        //error message
        return;
    }

    player = new Player();

    // read player datas from game data file
    int sPokemonSize;
    vector<int> sMoveSize;
    vector<string> sPokemonName;
    vector<vector<string>> sMoveName;
    gameDataData >> sPokemonSize;
    gameDataData.ignore();
    sMoveSize.assign(sPokemonSize, 0);
    sPokemonName.assign(sPokemonSize, "");
    sMoveName.assign(sPokemonSize, vector<string>());

    for (int i = 0; i < sPokemonSize; i++)
    {
        gameDataData >> sPokemonName[i] >> sMoveSize[i];
        gameDataData.ignore();
        vector<string> movesName;
        for (int j = 0; j < sMoveSize[i]; j++)
        {
            string temp;
            gameDataData >> temp;
            movesName.push_back(temp);
        }
        gameDataData.ignore();

        player->addPokemon(*gameLib, sPokemonName[i], movesName);
        sMoveName[i] = movesName;
    }

    oponent = new Player();

    // read oponent datas from game data file
    int ePokemonSize;
    vector<int> eMoveSize;
    vector<string> ePokemonName;
    vector<vector<string>> eMoveName;
    gameDataData >> ePokemonSize;
    gameDataData.ignore();
    eMoveSize.assign(ePokemonSize, 0);
    ePokemonName.assign(ePokemonSize, "");
    eMoveName.assign(ePokemonSize, vector<string>());

    for (int i = 0; i < ePokemonSize; i++)
    {
        gameDataData >> ePokemonName[i] >> eMoveSize[i];
        gameDataData.ignore();
        vector<string> movesName;
        for (int j = 0; j < eMoveSize[i]; j++)
        {
            string temp;
            gameDataData >> temp;
            movesName.push_back(temp);
        }
        gameDataData.ignore();

        oponent->addPokemon(*gameLib, ePokemonName[i], movesName);
        eMoveName[i] = movesName;
    }

    // set a new battle manager
    battle = new BattleManager(player, oponent);

    // read command from test case file
    while (!caseData.eof())
    {
        string command;
        getline(caseData, command);

        if (command == "Test") battle->setTestMode(); // set battle manager to test mode
        else if (command == "Battle") // both use move
        {
            string playerMove, oponentMove;
            getline(caseData, playerMove);
            getline(caseData, oponentMove);
            bool isVaild = true;

            // check pp of player pokemon move isn't 0
            for (int i = 0; i < player->getPokemons()[player->getCurrentPokemon()]->getMoves().size(); i++)
            {
                if (player->getPokemons()[player->getCurrentPokemon()]->getMoves()[i]->getName() == playerMove)
                {
                    if (player->getPokemons()[player->getCurrentPokemon()]->getMoves()[i]->getPp() <= 0)
                    {
                        isVaild = true;
                        break;
                    }
                }
            }

            // check pp of player pokemon move isn't 0
            for (int i = 0; i < oponent->getPokemons()[oponent->getCurrentPokemon()]->getMoves().size(); i++)
            {
                if (oponent->getPokemons()[oponent->getCurrentPokemon()]->getMoves()[i]->getName() == playerMove)
                {
                    if (oponent->getPokemons()[oponent->getCurrentPokemon()]->getMoves()[i]->getPp() <= 0)
                    {
                        isVaild = true;
                        break;
                    }
                }
            }

            if (isVaild) battle->move(playerMove, oponentMove);
        }
        else if (command == "Bag") // player use potion
        {
            string potion, pokemonName, oponentMove;
            getline(caseData, potion);
            getline(caseData, pokemonName);
            getline(caseData, oponentMove);

            Pokemon* playerPokemon = nullptr;
            for (auto& i : player->getPokemons())
                if (i->getName() == pokemonName)
                    playerPokemon = i;

            battle->usePotion(potion, *playerPokemon, oponentMove);
        }
        else if (command == "Pokemon") // player swap pokemon
        {
            string pokemonName, oponentMove;
            getline(caseData, pokemonName);
            getline(caseData, oponentMove);

            int i = 0;
            for (; i < player->getPokemons().size(); i++)
                if (player->getPokemons()[i]->getName() == pokemonName)
                    break;

            battle->swap(i, oponentMove);
        }
        else if (command == "Status") // check player pokemon and oponent pokemon state
        {
            Pokemon& playerPokemon = *player->getPokemons()[player->getCurrentPokemon()];
            Pokemon& oponentPokemon = *oponent->getPokemons()[oponent->getCurrentPokemon()];

            cout << "[Turn " << battle->getTurn() << "] ";
            cout << playerPokemon.getName() << " " << playerPokemon.getCurrentHp();
            for (int i = NONE; i <= PSN; i++)
                if (playerPokemon.getStatus()[i])
                {
                    if (i == PAR) cout << " PAR";
                    else if (i == BUR) cout << " BRN";
                    else if (i == PSN) cout << " PSN";
                }

            cout << " ";

            cout << oponentPokemon.getName() << " " << oponentPokemon.getCurrentHp();
            for (int i = NONE; i <= PSN; i++)
                if (oponentPokemon.getStatus()[i])
                {
                    if (i == PAR) cout << " PAR";
                    else if (i == BUR) cout << " BRN";
                    else if (i == PSN) cout << " PSN";
                }

            cout << endl;
        }
        else if (command == "Check") //  check player pokemon move and oponent pokemon move
        {
            vector<Move*>& playerPokemonMoves = player->getPokemons()[player->getCurrentPokemon()]->getMoves();

            cout << "[Turn " << battle->getTurn() << "] ";
            for (int i = 0; i < playerPokemonMoves.size(); i++)
            {
                cout << playerPokemonMoves[i]->getName() << " " << playerPokemonMoves[i]->getPp();
                if (i + 1 != playerPokemonMoves.size()) cout << " ";
            }

            cout << endl;
        }
        else if (command == "Run")
        {
            // exit;
        }
    }

    // write current battle data to a json
    pokemonDatas.clear();
    pokemonDatas.push_back(json::value());
    pokemonDatas.back()[U("PokemonNum")] = json::value(sPokemonSize);
    pokemonDatas.back()[U("sCurrentPokemon")] = json::value(player->getCurrentPokemon());
    string whichWin = "";

    // write game state
    switch (battle->getGameState())
    {
    case 0:
        pokemonDatas.back()[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
        break;
    case 1:
        whichWin = "sWin";
        pokemonDatas.back()[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
        break;
    case 2:
        whichWin = "eWin";
        pokemonDatas.back()[U("GameState")] = json::value(wstring(whichWin.begin(), whichWin.end()));
        break;
    }

    // get massage from output file
    string logs = battle->getMessage();
    pokemonDatas.back()[U("Info")] = json::value(wstring(logs.begin(), logs.end()));

    for (int i = 0; i < sPokemonSize; i++)
    {
        // write json of player pokemon and moves data
        pokemonDatas.push_back(json::value());
        Pokemon pokemon = *player->getPokemons()[i];
        pokemonDatas.back()[0][U("Name")] = json::value(wstring(sPokemonName[i].begin(), sPokemonName[i].end()));
        pokemonDatas.back()[0][U("MoveNum")] = json::value(sMoveSize[i]);
        pokemonDatas.back()[0][U("MaxHp")] = json::value(pokemon.getMaxHp());
        pokemonDatas.back()[0][U("CurrentHp")] = json::value(pokemon.getCurrentHp());
        pokemonDatas.back()[0][U("Atk")] = json::value(pokemon.getAtk());
        pokemonDatas.back()[0][U("Def")] = json::value(pokemon.getDef());
        pokemonDatas.back()[0][U("SpAtk")] = json::value(pokemon.getSpAtk());
        pokemonDatas.back()[0][U("SpDef")] = json::value(pokemon.getSpDef());
        pokemonDatas.back()[0][U("Speed")] = json::value(pokemon.getSpeed());
        string type = gLib->changeToTypeName(pokemon.getType()[0]);
        pokemonDatas.back()[0][U("Type1")] = json::value(wstring(type.begin(), type.end()));

        if (pokemon.getType().size() >= 2)
        {
            type = gLib->changeToTypeName(pokemon.getType()[1]);
            pokemonDatas.back()[0][U("Type2")] = json::value(wstring(type.begin(), type.end()));
        }

        // write pokemon effect data
        for (int j = 0; j < pokemon.getStatus().size(); j++)
        {
            switch (j)
            {
            case PAR:
                pokemonDatas.back()[0][U("sPAR")] = json::value(pokemon.getStatus()[PAR]);
                break;
            case BUR:
                pokemonDatas.back()[0][U("sBRN")] = json::value(pokemon.getStatus()[BUR]);
                break;
            case PSN:
                pokemonDatas.back()[0][U("sPSN")] = json::value(pokemon.getStatus()[PSN]);
                break;
            }
        }

        // write json of pokemon moves data
        for (int j = 1; j <= sMoveSize[i]; j++)
        {
            json::value moveDatas;
            Move move = *pokemon.getMoves()[j - 1];
            Move labMove = gLib->getMove(move.getName());
            moveDatas[U("MoveName")] = json::value(wstring(move.getName().begin(), move.getName().end()));
            string type = gLib->changeToTypeName(move.getType());
            moveDatas[U("MoveType")] = json::value(wstring(type.begin(), type.end()));
            moveDatas[U("MovePower")] = json::value(move.getPower());
            moveDatas[U("MoveAccuracy")] = json::value(move.getAccuracy());
            moveDatas[U("MoveCurrentPP")] = json::value(move.getPp());
            moveDatas[U("MoveMaxPP")] = json::value(labMove.getPp());
            string effect = gLib->changeToEffectName(move.getAdditionEffect());
            moveDatas[U("MoveEffect")] = json::value(wstring(effect.begin(), effect.end()));

            if (move.getPower() <= 0) moveDatas[U("MoveCategory")] = json::value(L"State");
            else if (move.getSpecialAttack()) moveDatas[U("MoveCategory")] = json::value(L"Special");
            else moveDatas[U("MoveCategory")] = json::value(L"Physical");

            pokemonDatas.back()[j] = moveDatas;
        }
    }

    // write json of oponent pokemon data
    for (int i = 0; i < ePokemonSize; i++)
    {
        if (i == oponent->getCurrentPokemon())
        {
            // write json of oponent first pokemon
            pokemonDatas.push_back(json::value());
            Pokemon pokemon = *oponent->getPokemons()[oponent->getCurrentPokemon()];
            pokemonDatas.back()[0][U("Name")] = json::value(wstring(ePokemonName[i].begin(), ePokemonName[i].end()));
            pokemonDatas.back()[0][U("MaxHp")] = json::value(pokemon.getMaxHp());
            pokemonDatas.back()[0][U("CurrentHp")] = json::value(pokemon.getCurrentHp());
            pokemonDatas.back()[0][U("Atk")] = json::value(pokemon.getAtk());
            pokemonDatas.back()[0][U("Def")] = json::value(pokemon.getDef());
            pokemonDatas.back()[0][U("SpAtk")] = json::value(pokemon.getSpAtk());
            pokemonDatas.back()[0][U("SpDef")] = json::value(pokemon.getSpDef());
            pokemonDatas.back()[0][U("Speed")] = json::value(pokemon.getSpeed());
            string type = gLib->changeToTypeName(pokemon.getType()[0]);
            pokemonDatas.back()[0][U("Type1")] = json::value(wstring(type.begin(), type.end()));

            if (pokemon.getType().size() >= 2)
            {
                type = gLib->changeToTypeName(pokemon.getType()[1]);
                pokemonDatas.back()[0][U("Type2")] = json::value(wstring(type.begin(), type.end()));
            }

            // write effect daata
            for (int j = 0; j < pokemon.getStatus().size(); j++)
            {
                switch (j)
                {
                case PAR:
                    pokemonDatas.back()[0][U("ePAR")] = json::value(pokemon.getStatus()[PAR]);
                    break;
                case BUR:
                    pokemonDatas.back()[0][U("eBRN")] = json::value(pokemon.getStatus()[BUR]);
                    break;
                case PSN:
                    pokemonDatas.back()[0][U("ePSN")] = json::value(pokemon.getStatus()[PSN]);
                    break;
                }
            }
        }
    }
}