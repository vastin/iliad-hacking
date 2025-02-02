#ifndef _VT_KERN_H
#define _VT_KERN_H

/*
 * this really is an extension of the vc_cons structure in console.c, but
 * with information needed by the vt package
 */

#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/tty.h>
#include <linux/console_struct.h>

/*
 * Presently, a lot of graphics programs do not restore the contents of
 * the higher font pages.  Defining this flag will avoid use of them, but
 * will lose support for PIO_FONTRESET.  Note that many font operations are
 * not likely to work with these programs anyway; they need to be
 * fixed.  The linux/Documentation directory includes a code snippet
 * to save and restore the text font.
 */
#ifdef CONFIG_VGA_CONSOLE
#define BROKEN_GRAPHICS_PROGRAMS 1
#endif

extern struct vt_struct {
	int vc_num;				/* The console number */
	unsigned char	vc_mode;		/* KD_TEXT, ... */
	struct vt_mode	vt_mode;
	int		vt_pid;
	int		vt_newvt;
	wait_queue_head_t paste_wait;
} *vt_cons[MAX_NR_CONSOLES];

extern void kd_mksound(unsigned int hz, unsigned int ticks);
extern int kbd_rate(struct kbd_repeat *rep);

/* console.c */

int vc_allocate(unsigned int console);
int vc_cons_allocated(unsigned int console);
int vc_resize(int currcons, unsigned int cols, unsigned int lines);
void vc_disallocate(unsigned int console);
void reset_palette(int currcons);
void set_palette(int currcons);
void do_blank_screen(int entering_gfx);
void do_unblank_screen(int leaving_gfx);
void unblank_screen(void);
void poke_blanked_console(void);
int con_font_op(int currcons, struct console_font_op *op);
int con_font_set(int currcons, struct console_font_op *op);
int con_font_get(int currcons, struct console_font_op *op);
int con_font_default(int currcons, struct console_font_op *op);
int con_font_copy(int currcons, struct console_font_op *op);
int con_set_cmap(unsigned char *cmap);
int con_get_cmap(unsigned char *cmap);
void scrollback(int);
void scrollfront(int);
void update_region(int currcons, unsigned long start, int count);
void redraw_screen(int new_console, int is_switch);
#define update_screen(x) redraw_screen(x, 0)
#define switch_screen(x) redraw_screen(x, 1)

struct tty_struct;
int tioclinux(struct tty_struct *tty, unsigned long arg);

/* consolemap.c */

struct unimapinit;
struct unipair;

int con_set_trans_old(unsigned char * table);
int con_get_trans_old(unsigned char * table);
int con_set_trans_new(unsigned short * table);
int con_get_trans_new(unsigned short * table);
int con_clear_unimap(int currcons, struct unimapinit *ui);
int con_set_unimap(int currcons, ushort ct, struct unipair *list);
int con_get_unimap(int currcons, ushort ct, ushort *uct, struct unipair *list);
int con_set_default_unimap(int currcons);
void con_free_unimap(int currcons);
void con_protect_unimap(int currcons, int rdonly);
int con_copy_unimap(int dstcons, int srccons);

/* vt.c */
void complete_change_console(unsigned int new_console);
int vt_waitactive(int vt);
void change_console(unsigned int);
void reset_vc(unsigned int new_console);

/*
 * vc_screen.c shares this temporary buffer with the console write code so that
 * we can easily avoid touching user space while holding the console spinlock.
 */
extern char con_buf[PAGE_SIZE];
#define CON_BUF_SIZE	PAGE_SIZE
extern struct semaphore con_buf_sem;

#endif /* _VT_KERN_H */
