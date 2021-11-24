#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_phasor_tilde_class;

typedef struct _rp_phasor_tilde
{
    t_object x_obj;
    t_outlet* x_out;
    Phasor * phasor;

    t_float f;
} t_rp_phasor_tilde;

t_int* rp_phasor_tilde_perform(t_int *w)
{
    t_rp_phasor_tilde* x = (t_rp_phasor_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float) * size);
    phasor_process(x->phasor, out, size);

    return (w+5);
}

void rp_phasor_tilde_dsp(t_rp_phasor_tilde* x, t_signal **sp)
{
    dsp_add(rp_phasor_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_phasor_tilde_new()
{
    t_rp_phasor_tilde *x = (t_rp_phasor_tilde *)pd_new(rp_phasor_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->phasor = phasor_create();
    return (void *)x;
}

void rp_phasor_tilde_free(t_rp_phasor_tilde *x)
{
    outlet_free(x->x_out);
    phasor_destroy(x->phasor);
}

void rp_phasor_tilde_dc(t_rp_phasor_tilde* x, t_float dc)
{
    phasor_set_duty_cycle(x->phasor, dc);
}

void rp_phasor_tilde_setup(void) {
    rp_phasor_tilde_class = class_new(gensym("rp_phasor~"),
                                     (t_newmethod)rp_phasor_tilde_new,
                                     (t_method)rp_phasor_tilde_free,
                                     sizeof(t_rp_phasor_tilde), CLASS_DEFAULT, 0);

    class_addmethod(rp_phasor_tilde_class, (t_method)rp_phasor_tilde_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(rp_phasor_tilde_class, (t_method)rp_phasor_tilde_dc, gensym("dc"), A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_phasor_tilde_class, t_rp_phasor_tilde, f);
}