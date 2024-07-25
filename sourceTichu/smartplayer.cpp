#include "player.h"


// an einai mono se ena syndiasmo dhladh einai single to vazei true
bool isJustSingle(Card* sdok, Hand* hand){
    int FY = 0;
    for(int i = 0; i != hand->getNumberOfCombinations(); i++){
        if(hand->getCombination(i)->containsCard(sdok)) FY++;
    }
    if (FY == 1) return true;
    return false;
}


SmartPlayer::SmartPlayer(int index) : Player(index) {team = "Team 033";}

int SmartPlayer::setPhoenixValue(Table* table, PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer, Combination* lastCombinationOnTable){
    //prepare to leave if we have only 2 cards left AND the lastCombinationOnTable is of Type PAIR or it is NULL (that is we have hand)

    if (lastCombinationOnTable != NULL){// EDW MPAINOUME AN DEN EXOUME XERI GOOD
        if(numberOfCardsPerPlayer[index] == 2 && lastCombinationOnTable->hasType(PAIR)){ //EDW ELEGXOUME AN MPOROUME NA VGOUME ME PAIR
            if(lastCombinationOnTable->getValue() < hand->getCard(1)->getValue()){
                if (!hand->getCard(1)->hasSuit(DRAGON))
                    return hand->getCard(1)->getValue();
                else
                    return 14;

            }
        }
        if(numberOfCardsPerPlayer[index] == 3 && lastCombinationOnTable->hasType(THREEOFAKIND)){ //ELEGXOUME TRIPLETA
                if(hand->getCombination(3)->hasType(PAIR))//THA TO ANTIGRAPSWWW
                    if(lastCombinationOnTable->getValue() < hand->getCard(1)->getValue())
                        return hand->getCard(1)->getValue();
        }
        //ama exei ace pame gia ace
        if (lastCombinationOnTable->getCard(0)->getValue() == 14) //ELEGXOUME ASO
            return 14;
        if (hand->getCardCount() >= 2 && hand->getCardCount() <= 4){
            int lastCard = hand->getCardCount() - 1;
            //index of lastCard
            if(hand->getCard(lastCard)->hasSuit(DRAGON)) lastCard--;
            if(lastCard == 0) return 14;
            //if we have phoenix or phoenix and dragon
            return hand->getCard(lastCard)->getValue();
        }
        //aim for high pair or three of a kind
        if (!lastCombinationOnTable->hasType(SINGLE)){
            int lastCard = hand->getCardCount() - 1;    //index of lastCard
            if (hand->getCard(lastCard)->hasSuit(DRAGON)) lastCard--;
            if (lastCard == 0) return 14; //if we have phoenix or phoenix and dragon
            return hand->getCard(lastCard)->getValue();
        }
    } else { // when you have hand
        if (hand->getCardCount() >= 2 && hand->getCardCount() <= 4){
            int lastCard = hand->getCardCount() - 1;
            //index of lastCard
            if(hand->getCard(lastCard)->hasSuit(DRAGON)) lastCard--;
            if(lastCard == 0) return 14;
            //if we have phoenix or phoenix and dragon
            return hand->getCard(lastCard)->getValue();
        }
        //make a beautiful STRAIGHT combination
        bool hasAtLeastOneCopyOf[14] = {0}; //0 is mahjong, 1 is two, ..., 12 is K, 13 is Ace
        for (int i = 0; i < hand->getCardCount(); i++) {
            if (hand->getCard(i)->hasSuit(PHOENIX) || hand->getCard(i)->hasSuit(DOG) ||
                    hand->getCard(i)->hasSuit(DRAGON)) continue;

            hasAtLeastOneCopyOf[((hand->getCard(i)->getValue()) - 1)] = true;
        }
        int flag = 0;
        int counter = 0;
        int gap;
        for (int i = 0; i < 14; i++) {
            if(hasAtLeastOneCopyOf[i]) {
                counter++;
            } else {
                flag++; // flag becomes 1 if we have only one gap and 2 if we have two gaps
                if (flag == 2) {
                    if (counter >= 4) { //if the phoenix can make a playable STRAIGHT combination return
                        return (gap + 1);
                    }
                    else {
                        flag = 0;
                        counter = 0;
                    }

                }
                gap = i;
            }
        }
        if (flag == 1) // if we had a single gap
        return (gap + 1);
    }
    return 14;
}






int SmartPlayer::decideAndPlay(Combination** playableCombinations, int numPlayableCombinations, Table* table,
                                   PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer, Combination* lastCombinationOnTable){

    //an mporeis na vgeis, vges
    for(int i = 0; i != numPlayableCombinations; i++){
        if (playableCombinations[i]->getNumberOfCards() == hand->getCardCount()) return i;
    }

    //if any of our useless opponents has one card we blast
    if (numberOfCardsPerPlayer[(index + 1) % 4] == 1 || numberOfCardsPerPlayer[(index + 3) % 4] == 1) return (numPlayableCombinations - 1);

    //get which cards can only be played as Singles, generally, not with this last combination on table
    bool SinglesOnly[numPlayableCombinations];
    for(int i = 0; i != numPlayableCombinations; i++) {SinglesOnly[i] = false;}
    for(int i = 0; i != numPlayableCombinations; i++){
        if (!playableCombinations[i]->hasType(SINGLE)) break;
        SinglesOnly[i] = isJustSingle(playableCombinations[i]->getCard(0), hand);
    }

    //if teammate has played last a relatively low Single, we hit in some cases
    if (playerStatuses[(index + 2) % 4] == PLAYED){
        if (lastCombinationOnTable->hasType(SINGLE) && lastCombinationOnTable->getValue() < 8){
            for(int i = 0; i != numPlayableCombinations; i++){
                if (SinglesOnly[i] && playableCombinations[i]->getValue() < 9) return i;
            }
        }
        return -1;
    }


    //if we won the round
    if (lastCombinationOnTable == NULL){
        //play a single without destroying other strong combinations
        //we prefer starting with single hoping some other teams play single destroying other important combinations
        for(int i = 0; i != numPlayableCombinations; i++){
            if (SinglesOnly[i] && playableCombinations[i]->getValue() < 9) return i;
        }
        //play straight if we have, straight is good
        for(int i = numPlayableCombinations - 1; i != -1; i--){
            if (playableCombinations[i]->hasType(STRAIGHT)) return i;
        }
        //play stairs if we have, stairs are good
        for(int i = numPlayableCombinations - 1; i != -1; i--){
            if (playableCombinations[i]->hasType(STAIRS)) return i;
        }
        //play full house if we have, full house is good
        for(int i = 0; i != numPlayableCombinations; i++){
            if (playableCombinations[i]->hasType(FULLHOUSE)) return i;
        }
        //play three of a kind if we have, three of a kind is not bad
        for(int i = 0; i != numPlayableCombinations; i++){
            if (playableCombinations[i]->hasType(THREEOFAKIND)) return i;
        }
        //play pair if we have, h agaph 8elei duo
        for(int i = 0; i != numPlayableCombinations; i++){
            if (playableCombinations[i]->hasType(PAIR)) return i;
        }
        //play something if not already
        return 0;
    }

    //if last combination is single
    if (lastCombinationOnTable->hasType(SINGLE)){
        for(int i = 0; i != numPlayableCombinations; i++){
            if (SinglesOnly[i] ||
            (playableCombinations[i]->getValue() > 12 && playableCombinations[i]->hasType(SINGLE))) return i;
        }
        if (lastCombinationOnTable->getCard(0)->hasSuit(DRAGON)) return 0; //hit dragon with bomb
        return -1;
    }
    //play
    return 0;
    return 420; //hehe weedidit
}


void SmartPlayer::watch(Table* table, PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer){
}
