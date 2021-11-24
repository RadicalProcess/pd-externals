#include "m_pd.h"
#include <string.h>
#include <dsp.h>

static t_class *rp_wintp_tilde_class;

typedef struct _rp_wintp_tilde
{
    t_object x_obj;
    t_outlet* x_out;
    WaveInterpolator * waveInterpolator;

    t_float f;
} t_rp_wintp_tilde;

t_int* rp_wintp_tilde_perform(t_int *w)
{
    t_rp_wintp_tilde* x = (t_rp_wintp_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float ) * size);
    wintp_process(x->waveInterpolator, out, size);

    return (w+5);
}

void rp_wintp_tilde_dsp(t_rp_wintp_tilde* x, t_signal **sp)
{
    dsp_add(rp_wintp_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_wintp_tilde_new(void)
{
    t_rp_wintp_tilde *x = (t_rp_wintp_tilde *)pd_new(rp_wintp_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->waveInterpolator = wintp_create();

    return (void *)x;
}

void rp_wintp_tilde_free(t_rp_wintp_tilde *x)
{
    outlet_free(x->x_out);

    wintp_destroy(x->waveInterpolator);
}

void rp_wintp_tilde_set(t_rp_wintp_tilde *x, t_float _x, t_float y){
    wintp_set(x->waveInterpolator, _x, y);
}


void rp_wintp_tilde_setup(void) {
    rp_wintp_tilde_class = class_new(gensym("rp_wintp~"),
                                     (t_newmethod)rp_wintp_tilde_new,
                                     (t_method)rp_wintp_tilde_free,
                                     sizeof(t_rp_wintp_tilde), CLASS_DEFAULT, 0);


    class_addmethod(rp_wintp_tilde_class, (t_method)rp_wintp_tilde_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod(rp_wintp_tilde_class, (t_method)rp_wintp_tilde_set, gensym("set"), A_FLOAT, A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_wintp_tilde_class, t_rp_wintp_tilde, f);
}