NEURON {
    POINT_PROCESS exp2isyn
    RANGE tau1, tau2
    NONSPECIFIC_CURRENT i
}

UNITS {
    (mV) = (millivolt)
}

PARAMETER {
    tau1 = 0.5 (ms)
    tau2 = 2   (ms)
}

ASSIGNED {
    factor
}

STATE {
    A B
}

INITIAL {
    LOCAL tp
    A = 0
    B = 0
    tp = (tau1*tau2)/(tau2 - tau1) * log(tau2/tau1)
    factor = 1 / (-exp(-tp/tau1) + exp(-tp/tau2))
}

BREAKPOINT {
    SOLVE state METHOD cnexp
    i = (B - A)
}

DERIVATIVE state {
    A' = -A/tau1
    B' = -B/tau2
}

NET_RECEIVE(weight) {
    A = A + weight*factor
    B = B + weight*factor
}
