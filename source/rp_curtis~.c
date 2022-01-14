#include "m_pd.h"
#include <string.h>
#include <curtis_core/CurtisAPI.h>

static t_class *rp_curtis_tilde_class;

typedef struct _rp_curtis_tilde
{
    t_object x_obj;
    t_outlet* x_out;
    CurtisCore * curtis_core;
    t_float f;
} t_rp_curtis_tilde;

t_int* rp_curtis_tilde_perform(t_int *w)
{
    t_rp_curtis_tilde* x = (t_rp_curtis_tilde *)(w[1]);
    t_sample* in = (t_sample*)(w[2]);
    t_sample* out = (t_sample*)(w[3]);
    uintptr_t size = (uintptr_t)(w[4]);

    memcpy(out, in, sizeof(float) * size);
    curtis_process(x->curtis_core, out, size);

    return (w+5);
}

void rp_curtis_tilde_dsp(t_rp_curtis_tilde* x, t_signal **sp)
{
    dsp_add(rp_curtis_tilde_perform, 4,
            x,
            sp[0]->s_vec,
            sp[1]->s_vec,
            sp[0]->s_n);
}

void *rp_curtis_tilde_new()
{
    t_rp_curtis_tilde *x = (t_rp_curtis_tilde *)pd_new(rp_curtis_tilde_class);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    x->curtis_core = curtis_create(sys_getsr());
    return (void *)x;
}

void rp_curtis_tilde_free(t_rp_curtis_tilde *x)
{
    outlet_free(x->x_out);
    curtis_destroy(x->curtis_core);
}

void rp_curtis_tilde_segment_min_length(t_rp_curtis_tilde *x, t_float length)
{
    curtis_segment_min_length(x->curtis_core, length);
}

void rp_curtis_tilde_repeat_min(t_rp_curtis_tilde *x, t_float count)
{
    curtis_repeat_min(x->curtis_core, (uintptr_t)count);
}

void rp_curtis_tilde_repeat_max(t_rp_curtis_tilde *x, t_float count)
{
    curtis_repeat_max(x->curtis_core, (uintptr_t)count);
}

void rp_curtis_tilde_random_range(t_rp_curtis_tilde *x, t_float range)
{
    curtis_random_range(x->curtis_core, (uintptr_t)range);
}

void rp_curtis_tilde_glisson_enabled(t_rp_curtis_tilde *x, t_float enabled)
{
    curtis_glisson_enabled(x->curtis_core, (bool)enabled);
}

void rp_curtis_tilde_start_min_speed(t_rp_curtis_tilde *x, t_float speed)
{
    curtis_start_min_speed(x->curtis_core, speed);
}

void rp_curtis_tilde_start_max_speed(t_rp_curtis_tilde *x, t_float speed)
{
    curtis_start_max_speed(x->curtis_core, speed);
}

void rp_curtis_tilde_end_min_speed(t_rp_curtis_tilde *x, t_float speed)
{
    curtis_end_min_speed(x->curtis_core, speed);
}

void rp_curtis_tilde_end_max_speed(t_rp_curtis_tilde *x, t_float speed)
{
    curtis_end_max_speed(x->curtis_core, speed);
}

void rp_curtis_tilde_density(t_rp_curtis_tilde *x, t_float density)
{
    curtis_density(x->curtis_core, density);
}

void rp_curtis_tilde_setup(void) {
    rp_curtis_tilde_class = class_new(gensym("rp_curtis~"),
                                        (t_newmethod)rp_curtis_tilde_new,
                                        (t_method)rp_curtis_tilde_free,
                                        sizeof(t_rp_curtis_tilde), CLASS_DEFAULT, 0);

    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_dsp, gensym("dsp"), A_CANT, 0);

    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_segment_min_length, gensym("segmin"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_repeat_min, gensym("repmin"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_repeat_max, gensym("repmax"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_random_range, gensym("random"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_glisson_enabled, gensym("glisson"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_start_min_speed, gensym("startmin"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_start_max_speed, gensym("startmax"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_end_min_speed, gensym("endmin"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_end_max_speed, gensym("endmax"), A_FLOAT, 0);
    class_addmethod(rp_curtis_tilde_class, (t_method)rp_curtis_tilde_density, gensym("density"), A_FLOAT, 0);

    CLASS_MAINSIGNALIN(rp_curtis_tilde_class,t_rp_curtis_tilde, f);
}
