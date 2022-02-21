#include "graph.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    graph_t *g = load_graph();

    queue_t *q = allocate_queue(1 + g->nodec / 8);

    qmem_t root = {.node_index = 0, .prev_feed = g->friendly_feed + 1, .prev_packs = 0};
    queue_push(q, &root);

    qmem_t pop;
    while ((pop = queue_pop(q)).node_index != -1)
    {
        node_t *n = &g->nodes[pop.node_index];
        int packs = pop.prev_packs;
        int feed = pop.prev_feed - 1;
        if (feed < 0)
        {
            packs += 1;
            feed = g->friendly_feed;
        }

        if (packs > n->least_packs || (packs == n->least_packs && feed <= n->most_fed))
            continue;

        if (pop.node_index < g->friendlyc)
            feed = g->friendly_feed;

        n->least_packs = packs;
        n->most_fed = feed;

        for (int i = 0; i < n->neighbour_count; ++i)
        {
            qmem_t new = {
                .node_index = n->neighbours[i],
                .prev_feed = feed,
                .prev_packs = packs};
            queue_push(q, &new);
        }
    }

    free_queue(q);

    int most_packs = 0;
    int zero_packs_count = 0;
    for (int i = 0; i < g->nodec; ++i)
    {
        node_t *n = &g->nodes[i];
        if (n->least_packs == 0)
        {
            zero_packs_count++;
        }
        else if (n->least_packs > most_packs)
        {
            most_packs = n->least_packs;
        }
    }
    printf("%d %d\r\n", most_packs, zero_packs_count);

    free_graph(g);
    return 0;
}