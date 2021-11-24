#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_hold_tilde_class;

typedef struct _rp_hold_tilde
{
    t_object x_obj;
    t_outlet* x_out;
    Hold* hold;

    t_float f;
} t_rp_hold_tilde;

t_int* rp_hold_tilde_perform(t_int *w)
{
    t_rp_hold_tilde* x = (t_rp_hold_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float) * size);
    hold_process(x->hold, out, size);

    return (w+5);
}

void rp_hold_tilde_dsp(t_rp_hold_tilde* x, t_signal **sp)
{
    dsp_add(rp_hold_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_hold_tilde_new()
{
    t_rp_hold_tilde *x = (t_rp_hold_tilde *)pd_new(rp_hold_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->hold = hold_create();
    return (void *)x;
}

void rp_hold_tilde_gate(t_rp_hold_tilde *x, t_float gate)
{
    hold_set_gate(x->hold, gate);
}

void rp_hold_tilde_thresh(t_rp_hold_tilde *x, t_float thresh)
{
    hold_set_thresh(x->hold, thresh);
}

void rp_hold_tilde_boost(t_rp_hold_tilde *x, t_float boost)
{
    hold_set_boost(x->hold, boost);
}

void rp_hold_tilde_free(t_rp_hold_tilde *x)
{
    outlet_free(x->x_out);
    hold_destroy(x->hold);
}

void rp_hold_tilde_setup(void) {
    rp_hold_tilde_class = class_new(gensym("rp_hold~"),
                                     (t_newmethod)rp_hold_tilde_new,
                                     (t_method)rp_hold_tilde_free,
                                     sizeof(t_rp_hold_tilde), CLASS_DEFAULT, 0);

    class_addmethod(rp_hold_tilde_class, (t_method)rp_hold_tilde_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(rp_hold_tilde_class, (t_method)rp_hold_tilde_gate, gensym("gate"),  A_FLOAT, 0);
    class_addmethod(rp_hold_tilde_class, (t_method)rp_hold_tilde_thresh, gensym("thresh"),  A_FLOAT, 0);
    class_addmethod(rp_hold_tilde_class, (t_method)rp_hold_tilde_boost, gensym("boost"),  A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_hold_tilde_class, t_rp_hold_tilde, f);
}