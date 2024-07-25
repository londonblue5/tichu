#include "card.h"

/*The constructor of the base class needs
no parameters, that's why it isn't called here*/

SimpleCard::SimpleCard(int value, CardSuit suit){
    this->value = value;
    this->suit = suit;
}

int SimpleCard::getPoints(){
    switch(this->getValue()){
        case 5:
        //If the card has value 5
            return 5;
        case 10:
        //If the card has value 10
            return 10;
        case 13:
        //If the card is a King(K with value 13)
            return 10;
        default:
        //The rest of the cards count for 0
            return 0;
    }
}

bool SimpleCard::canBeInCombination(){
    //All simple cards can participate in a combination
    return true;
}

bool SimpleCard::canBeInBomb(){
    //All simple cards can be part of a bomb
    return true;
}

SpecialCard::SpecialCard(CardSuit suit){
    this->suit = suit;
    switch(suit){
        case MAHJONG:
            value = 1;
            break;
        case DRAGON:
            value = 15;
            break;
    //Phoenix or Dogs or any other card than the above have value -1
        default:
            value = -1;

    }
}

int SpecialCard::getPoints(){
    switch(suit){
        case DRAGON:
        //Dragon counts for 25 points
            return 25;
        case PHOENIX:
        //Phoenix counts for -25 points
            return -25;
        default:
        //All other special cards count for 0
            return 0;
    }
}

bool SpecialCard::canBeInCombination(){
    switch(suit){
        //Phoenix can be part of a combination
        case PHOENIX:
            return true;
        //MahJong can be part of a combination
        case MAHJONG:
            return true;
        //All other special cards cannot participate in combinations
        default:
            return false;
    }
}

bool SpecialCard::canBeInBomb(){
    //No special card can be part of a bomb
    return false;
}
