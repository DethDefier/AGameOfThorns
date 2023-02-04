bool toggle = false;
enum signalStates {INERT, ONE, ONE_END, TWO, TWO_END, SWITCH, RESOLVE, END_FLOWER, END, RESET, FLOWER, ONE_FLOWER, TWO_FLOWER};
byte signalState = INERT;
byte modeTimer = 0;
byte modeTimerMax = 25;
bool turnOne = true;
bool value = false;
byte team = 0;
byte winningTeam = 0;
byte turnsLeft = 0;
byte cycle = 0;
byte decidingVariable = 0;
byte receivedFace = 0;
byte maxTurns = 12;

void setup() {
  randomize();
  byte coinFlip = random(1);
  if (coinFlip == 1) {
    turnOne = true;
  } 
}

void inertLoop() {
  cycle = sin8_C(millis()/4 % 256);
  if (buttonMultiClicked()) {
    value = !value;
    if (value) {
      team = 3;
      turnsLeft = maxTurns;
    } else {
      team = 0;
      turnsLeft = 0;
    }
  }
  if (buttonDoubleClicked()) {
    signalState = SWITCH;
    modeTimer = modeTimerMax;
  }
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == SWITCH) {
        signalState = SWITCH;
        modeTimer = modeTimerMax;
        break;
      } if (getLastValueReceivedOnFace(f) == END_FLOWER || getLastValueReceivedOnFace(f) == END) {
        if (team == 3) {
          signalState = END_FLOWER;
        } else {
          signalState = END;
        }
        modeTimer = modeTimerMax;
        break;
      } if (getLastValueReceivedOnFace(f) == RESET) {
        signalState = RESET;
        modeTimer = modeTimerMax;
        break;
      } if ((getLastValueReceivedOnFace(f) == ONE) && (modeTimer == 0)) {
        signalState = ONE_END;
        receivedFace = f;
        modeTimer = modeTimerMax;
        break;
      } if ((getLastValueReceivedOnFace(f) == TWO) && (modeTimer == 0)) {
        signalState = TWO_END;
        receivedFace = f;
        modeTimer = modeTimerMax;
        break;
      } if (getLastValueReceivedOnFace(f) == FLOWER) {
        if (team == 1) {
          setValueSentOnFace(ONE_FLOWER, f);
          setColorOnFace(makeColorRGB(cycle,cycle,255),f);
        }
        if (team == 2) {
          setValueSentOnFace(TWO_FLOWER, f);
          setColorOnFace(makeColorRGB(255,cycle,cycle),f);
        }
      } 
    }
  }
  if ((buttonSingleClicked()) && (!value)) {
    if (turnOne) {
      signalState = ONE;
      modeTimer = modeTimerMax;
      team = 1;
    } else {
      signalState = TWO;
      modeTimer = modeTimerMax;
      team == 2;
    }
  }
}

void switchLoop() {
  signalState = RESOLVE;
  
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == INERT) {
        signalState = SWITCH;
      }
    }
  }
  setColor(WHITE);
  if (signalState == RESOLVE) {
    modeTimer = modeTimerMax;
    turnOne = !turnOne;
    if (team == 3) {
      turnsLeft--;
      if (turnsLeft < 1) {
        signalState = END_FLOWER;
      }
    }
  }
}

void oneLoop() {
  signalState = RESOLVE;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == INERT || modeTimer > 0) {
        signalState = ONE;
      }
    }
  }
  team = 1;
}

void twoLoop() {
  signalState = RESOLVE;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == INERT || modeTimer > 0) {
        signalState = TWO;
      }
    }
  }
  team = 2;
}

void resolveLoop() {
  signalState = INERT;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
       if (getLastValueReceivedOnFace(f) == SWITCH || getLastValueReceivedOnFace(f) == RESET || getLastValueReceivedOnFace(f) == ONE || getLastValueReceivedOnFace(f) == TWO || modeTimer > 0) {
        signalState = RESOLVE;
      }
      if ((getLastValueReceivedOnFace(f) == RESET) && (team == 3)) {
        team == 0;
        signalState = RESET;
        modeTimer = modeTimerMax;
        break;
      }
    }
  }
}

void displaySignalState() {
  if (team == 0) {
    FOREACH_FACE(f) {
      toggle = !toggle;
      if (toggle) {
        setColorOnFace(GREEN,f);
      } else {
        if (turnOne) {
          setColorOnFace(dim(BLUE,125),f);
        } else {
          setColorOnFace(dim(RED,125),f);
        }
      }
    }
  }
  if (team == 1) {
    FOREACH_FACE(f) {
      toggle = !toggle;
      if (toggle) {
        setColorOnFace(BLUE,f);
      } else {
        if (turnOne) {
          setColorOnFace(dim(BLUE,125),f);
        } else {
          setColorOnFace(dim(RED,125),f);
        }
      }
    }
  }
  if (team == 2) {
    FOREACH_FACE(f) {
      toggle = !toggle;
      if (toggle) {
        setColorOnFace(RED,f);
      } else {
        if (turnOne) {
          setColorOnFace(dim(BLUE,125),f);
        } else {
          setColorOnFace(dim(RED,125),f);
        }
      }
    }
  }
  if (team == 3) {
    FOREACH_FACE(f) {
      toggle = !toggle;
      if (toggle) {
        if (turnOne) {
          setColorOnFace(dim(BLUE,125),f);
        } else {
          setColorOnFace(dim(RED,125),f);
        }
      } else {
        if (winningTeam == 1) {
          setColorOnFace(makeColorRGB(cycle,0,255),f);
        } if (winningTeam == 2) {
          setColorOnFace(makeColorRGB(255,0,cycle),f);
        } if (winningTeam == 0) {
          setColorOnFace(MAGENTA,f);
        }
      }
      if (getLastValueReceivedOnFace(f) == RESET) {
        signalState = RESET;
        modeTimer = modeTimerMax;
        break;
      }
    }
  }
}

void endLoop() {
  FOREACH_FACE(f) {
    toggle = !toggle;
    if (toggle) {
      setColorOnFace(WHITE,f);
    } else {
      setColorOnFace(dim(WHITE, 100), f);
    }
    if (getLastValueReceivedOnFace(f) == RESET) {
      signalState = RESET;
      modeTimer = modeTimerMax;
      break;
    }
  }
  toggle = !toggle;
}

void endFlowerLoop() {
  FOREACH_FACE(f) {
    if (getLastValueReceivedOnFace(f) == RESET) {
      signalState = RESET;
      modeTimer = modeTimerMax;
      break;
    }
  }
  if (winningTeam == 0) {
    setColor(MAGENTA);
  }
  if (winningTeam == 1) {
    setColor(BLUE);
  }
  if (winningTeam == 2) {
    setColor(RED);
  }
}

void resetLoop() {
  signalState = RESOLVE;
  
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      if (!getLastValueReceivedOnFace(f) == RESET) {
        signalState = RESET;
      } else if (modeTimer > 0) {
        signalState = RESET;
      } 
    }
  }
  setColor(WHITE);
  if (signalState == RESOLVE) {
    modeTimer = 0;
    value = false;
    team = 0;
    turnOne = true;
  }
}

void oneEndLoop() {
  signalState = RESOLVE;
  if (team == 0) {
    team = 1;
  } if (team == 2) {
    team = 0;
  }
  modeTimer = modeTimerMax;
}

void twoEndLoop() {
  signalState = RESOLVE;
  if (team == 0) {
    team = 2;
  } if (team == 1) {
    team = 0;
  }
  modeTimer = modeTimerMax;
}

void loop() {
  displaySignalState();
  switch(signalState) {
    case INERT:
      inertLoop();
      break;
    case ONE:
      oneLoop();
      break;
    case ONE_END:
      oneEndLoop();
      break;
    case TWO:
      twoLoop();
      break;
    case TWO_END:
      twoEndLoop();
      break;
    case SWITCH:
      switchLoop();
      break;
    case RESOLVE:
      resolveLoop();
      break;
    case END_FLOWER:
      endFlowerLoop();
      break;
    case END:
      endLoop();
      break;
    case RESET:
      resetLoop();
      break;
  }
  if (buttonSingleClicked()) {
    
  }
  if (buttonDoubleClicked()) {
    
  }
  if (buttonLongPressed()) {
    signalState = RESET;
    modeTimer = modeTimerMax;
  }
  if (modeTimer > 0) {
    modeTimer--;
  }
  setValueSentOnAllFaces(signalState);
  if ((team == 3) && (signalState == INERT)) {
    decidingVariable = 6;
    setValueSentOnAllFaces(FLOWER);
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        if (getLastValueReceivedOnFace(f) == ONE_FLOWER) {
          decidingVariable--;
        }
        if (getLastValueReceivedOnFace(f) == TWO_FLOWER) {
          decidingVariable++;
        }
      }
    }
    if (decidingVariable < 6) {
      winningTeam = 1;
    } else if (decidingVariable > 6) {
      winningTeam = 2;
    } else {
      winningTeam = 0;
    }
  }
  if (signalState == INERT) {
    FOREACH_FACE(f) {
      if (getLastValueReceivedOnFace(f) == FLOWER) {
        if (team == 1) {
          setValueSentOnFace(ONE_FLOWER, f);
          setColorOnFace(makeColorRGB(cycle,cycle,255),f);
        }
        if (team == 2) {
          setValueSentOnFace(TWO_FLOWER, f);
          setColorOnFace(makeColorRGB(255,cycle,cycle),f);
        }
      } 
    }
  }
}
