#include <iostream>

#include "cycle_timer.h"
#include "gomoku_game.h"
#include "heuristic_solver.h"
#include "sequential_minimax_solver.h"
#include "sequential_nocopy_minimax_solver.h"
#include "sequential_alphabeta_solver.h"
#include "sequential_nocopy_alphabeta_solver.h"
#include "omp_contention_minimax_solver.h"
#include "omp_local_minimax_solver.h"
#include "omp_contention_alphabeta_solver.h"
#include "omp_local_alphabeta_solver.h"
#include "pvs_contention_alphabeta_solver.h"
#include "pvs_rand_contention_alphabeta_solver.h"
#include "pvs_local_alphabeta_solver.h"
#include "pvs_rand_local_alphabeta_solver.h"
#include "pvs_heur_contention_alphabeta_solver.h"

#define REPEAT_TRIALS 1
#define PLIES 3
// TODO: Tightly coupled with the size of the board. Very bad. Ugh.
#define TURNS 20

void runTrials(GameSolver<GomokuGame>& solver, const char* name) {
    std::cout << "----- " << name << " -----" << std::endl;
    double total_time = 0;
    std::cout << "Completed trials: ";
    for (int trial = 0; trial < REPEAT_TRIALS; ++trial) {
        solver.resetState();
        double trial_start = CycleTimer::currentSeconds();
        for (int i = 0; i < TURNS; ++i) {
            solver.playBestMove();
        }
        double trial_end = CycleTimer::currentSeconds();
        total_time += trial_end - trial_start;
        if (trial > 0) {
            std::cout << ", ";
        }
        std::cout << trial;
    }
    std::cout << std::endl;
    std::cout << "Seconds elapsed playing moves: " << total_time << std::endl
            << std::endl;
}

int main() {
    std::cout << "BOARD_SIDE=" << BOARD_SIDE << std::endl;
    std::cout << "K_IN_ROW=" << K_IN_ROW << std::endl;
    std::cout << "REPEAT_TRIALS=" << REPEAT_TRIALS << std::endl;
    std::cout << "PLIES=" << PLIES << std::endl;
    std::cout << "TURNS=" << TURNS << std::endl;

    HeuristicSolver<GomokuGame> hs;
    runTrials(hs, "HEURISTIC");

    SequentialMinimaxSolver<GomokuGame, PLIES> sms;
    runTrials(sms, "SEQUENTIAL MINIMAX");

    SequentialNoCopyMinimaxSolver<GomokuGame, PLIES> sncms;
    runTrials(sncms, "SEQUENTIAL (NO COPY) MINIMAX");

    SequentialAlphaBetaSolver<GomokuGame, PLIES> sabs;
    runTrials(sabs, "SEQUENTIAL ALPHA-BETA");

    SequentialNoCopyAlphaBetaSolver<GomokuGame, PLIES> sncabs;
    runTrials(sncabs, "SEQUENTIAL (NO COPY) ALPHA-BETA");

    OmpContentionMinimaxSolver<GomokuGame, PLIES> ocms;
    runTrials(ocms, "OPENMP (GLOBAL CONTENTION) MINIMAX");

    OmpLocalMinimaxSolver<GomokuGame, PLIES> olms;
    runTrials(olms, "OPENMP (THREAD-LOCAL) MINIMAX");

    OmpContentionAlphaBetaSolver<GomokuGame, PLIES> ocabs;
    runTrials(ocabs, "OPENMP (GLOBAL CONTENTION) ALPHA-BETA");

    OmpLocalAlphaBetaSolver<GomokuGame, PLIES> olabs;
    runTrials(olabs, "OPENMP (THREAD-LOCAL) ALPHA-BETA");

    PVSContentionAlphaBetaSolver<GomokuGame, PLIES> pcabs;
    runTrials(pcabs, "PVS (GLOBAL CONTENTION) ALPHA-BETA");

    PVSRandContentionAlphaBetaSolver<GomokuGame, PLIES> prcabs;
    runTrials(prcabs, "PVS (RAND, GLOBAL CONTENTION) ALPHA-BETA");

    PVSLocalAlphaBetaSolver<GomokuGame, PLIES> plabs;
    runTrials(plabs, "PVS (THREAD-LOCAL) ALPHA-BETA");

    PVSRandLocalAlphaBetaSolver<GomokuGame, PLIES> prlabs;
    runTrials(prlabs, "PVS (RAND, THREAD-LOCAL) ALPHA-BETA");

    PVSHeurContentionAlphaBetaSolver<GomokuGame, PLIES> phcabs;
    runTrials(phcabs, "PVS (HEUR, GLOBAL CONTENTION) ALPHA-BETA");

    return 0;
}
