NEURON {
    POINT_PROCESS expisyn
    RANGE tau
    NONSPECIFIC_CURRENT i
}

UNITS {
    (mV) = (millivolt)
}

PARAMETER {
    tau = 2.0 (ms)
}

ASSIGNED {}

STATE {
    g
}

INITIAL {
    g=0
}

BREAKPOINT {
    SOLVE state METHOD cnexp
    i = g
}

DERIVATIVE state {
    g' = -g/tau
}

NET_RECEIVE(weight) {
    g = g + weight
}
