/*
 *  filesel.c
 * 
 *  File Browser
 *
 *  Copyright Eke-Eke (2009-2014)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#include "shared.h"
#include "filesel.h"
#include "font.h"
#include "gui.h"
#include "file_load.h"
#include "history.h"

#define BG_COLOR_1 {0x49,0x49,0x49,0xff}
#define BG_COLOR_2 {0x66,0x66,0x66,0xff}

#define SCROLL_SPEED 10

extern const u8 Browser_dir_png[];
extern const u8 Snap_empty_png[];
extern const u8 Cart_md_png[];
extern const u8 Cart_ms_png[];
extern const u8 Cart_gg_png[];
extern const u8 Cart_sg_png[];

FILEENTRIES filelist[MAXFILES];

static int offset        = 0;
static int selection     = 0;
static int maxfiles      = 0;
static int string_offset = 0;
static char prev_folder[MAXJOLIET];
static void selector_cb(void);

/*****************************************************************************/
/*  GUI Buttons data                                                         */
/*****************************************************************************/
static butn_data arrow_up_data =
{
  {NULL,NULL},
  {Button_up_png,Button_up_over_png}
};

static butn_data arrow_down_data =
{
  {NULL,NULL},
  {Button_down_png,Button_down_over_png}
};

/*****************************************************************************/
/*  GUI Arrows button                                                        */
/*****************************************************************************/

static gui_butn arrow_up = {&arrow_up_data,BUTTON_VISIBLE|BUTTON_ACTIVE|BUTTON_OVER_SFX,{0,0,0,0},14,76,360,32};
static gui_butn arrow_down = {&arrow_down_data,BUTTON_VISIBLE|BUTTON_ACTIVE|BUTTON_OVER_SFX,{0,0,0,0},14,368,360,32};

/*****************************************************************************/
/*  GUI helpers                                                              */
/*****************************************************************************/
static gui_item action_cancel =
{
  NULL,Key_B_png,"","Previous Directory",10,422,28,28
};

static gui_item action_select =
{
  NULL,Key_A_png,"","Load file",602,422,28,28
};

/*****************************************************************************/
/*  GUI Background images                                                    */
/*****************************************************************************/
static gui_image bg_filesel[14] =
{
  {NULL,Bg_layer_png,IMAGE_VISIBLE|IMAGE_REPEAT,0,0,640,480,255},
  {NULL,Bg_overlay_png,IMAGE_VISIBLE|IMAGE_REPEAT,0,0,640,480,255},
  {NULL,Banner_top_png,IMAGE_VISIBLE,0,0,640,108,255},
  {NULL,Banner_bottom_png,IMAGE_VISIBLE,0,380,640,100,255},
  {NULL,Main_logo_png,IMAGE_VISIBLE,466,40,152,44,255},
  {NULL,Frame_s1_png,IMAGE_VISIBLE,8,70,372,336,152},
  {NULL,Frame_s2_png,0,384,264,248,140,152},
  {NULL,Snap_empty_png,IMAGE_VISIBLE,424,148,160,112,255},
  {NULL,NULL,0,424,148,160,112,255},
  {NULL,NULL,0,388,147,240,152,255},
  {NULL,NULL,0,388,147,240,152,255},
  {NULL,NULL,0,392,118,232,148,255},
  {NULL,NULL,0,414,116,184,188,255},
  {NULL,NULL,0,416,144,180,228,255}
};

static const u8 *Cart_png[FILETYPE_MAX] =
{
  Cart_md_png,
  Cart_md_png,
  Cart_ms_png,
  Cart_gg_png,
  Cart_sg_png
};

static const char *Cart_dir[FILETYPE_MAX] =
{
  "md",
  "cd",
  "ms",
  "gg",
  "sg"
};

static gui_image dir_icon;
static gui_image bar_over;

/*****************************************************************************/
/*  GUI Descriptor                                                           */
/*****************************************************************************/
static gui_menu menu_selector =
{
  "Game Selection",
  -1,-1,
  0,0,14,0,
  NULL,
  NULL,
  bg_filesel,
  {&action_cancel, &action_select},
  {&arrow_up,&arrow_down},
  selector_cb
};


static void selector_cb(void)
{
  int i;
  char text[MAXPATHLEN];
  int yoffset = 108;

  /* Draw browser array */
  gxDrawRectangle(15, 108, 358, 26, 127, (GXColor)BG_COLOR_1);
  gxDrawRectangle(15, 134, 358, 26, 127, (GXColor)BG_COLOR_2);
  gxDrawRectangle(15, 160, 358, 26, 127, (GXColor)BG_COLOR_1);
  gxDrawRectangle(15, 186, 358, 26, 127, (GXColor)BG_COLOR_2);
  gxDrawRectangle(15, 212, 358, 26, 127, (GXColor)BG_COLOR_1);
  gxDrawRectangle(15, 238, 358, 26, 127, (GXColor)BG_COLOR_2);
  gxDrawRectangle(15, 264, 358, 26, 127, (GXColor)BG_COLOR_1);
  gxDrawRectangle(15, 290, 358, 26, 127, (GXColor)BG_COLOR_2);
  gxDrawRectangle(15, 316, 358, 26, 127, (GXColor)BG_COLOR_1);
  gxDrawRectangle(15, 342, 358, 26, 127, (GXColor)BG_COLOR_2);

  /* Draw Files list */
  for (i = offset; (i < (offset + 10)) && (i < maxfiles); i++)
  {
    if (i == selection)
    {
      /* selection bar */
      gxDrawTexture(bar_over.texture,bar_over.x,yoffset+bar_over.y,bar_over.w,bar_over.h,255);

      /* scrolling text */
      if ((string_offset/SCROLL_SPEED) >= strlen(filelist[i].filename))
      {
        string_offset = 0;
      }

      if (string_offset)
      {
        sprintf(text,"%s ",filelist[i].filename+string_offset/SCROLL_SPEED);
        strncat(text, filelist[i].filename, string_offset/SCROLL_SPEED);
      }
      else
      {
        strcpy(text, filelist[i].filename);
      }

      /* print text */
      if (filelist[i].flags)
      {
        /* directory icon */
        gxDrawTexture(dir_icon.texture,dir_icon.x,yoffset+dir_icon.y,dir_icon.w,dir_icon.h,255);
        if (FONT_write(text,18,dir_icon.x+dir_icon.w+6,yoffset+22,bar_over.w-dir_icon.w-26,(GXColor)WHITE))
        {
          /* text scrolling */
          string_offset ++;
        }
      }
      else
      {
        if (FONT_write(text,18,26,yoffset+22,bar_over.w-20,(GXColor)WHITE))
        {
          /* text scrolling */
          string_offset ++;
        }
      }
    }
    else
    {
      if (filelist[i].flags)
      {
        /* directory icon */
        gxDrawTexture(dir_icon.texture,dir_icon.x,yoffset+dir_icon.y,dir_icon.w,dir_icon.h,255);
        FONT_write(filelist[i].filename,18,dir_icon.x+dir_icon.w+6,yoffset+22,bar_over.w-dir_icon.w-26,(GXColor)WHITE);
      }
      else
      {
        FONT_write(filelist[i].filename,18,26,yoffset+22,bar_over.w-20,(GXColor)WHITE);
      }
    }

    yoffset += 26;
  }
}  
  

/****************************************************************************
 * FileSelector
 *
 * Browse directories and select a file from the file listing
 * return ROM size
 *
 ****************************************************************************/ 
int FileSelector(int type)
{
  short p;
  int i;
  int old = -1;
  char fname[MAXPATHLEN];
  FILE *snap;
  gui_menu *m = &menu_selector;

  int offset_ = -1;
  int selection_ = -1;
  int maxfiles_ = -1;

#ifdef HW_RVL
  int x,y;
  gui_butn *button;
#endif

  /* Background overlay */
  if (config.bg_overlay)
  {
    bg_filesel[1].state |= IMAGE_VISIBLE;
  }
  else
  {
    bg_filesel[1].state &= ~IMAGE_VISIBLE;
  }

  /* Hide all cartridge labels */
  for (i=0; i<FILETYPE_MAX; i++)
  {
    bg_filesel[9+i].data = NULL;
    bg_filesel[9+i].state  &= ~IMAGE_VISIBLE;
  }

  /* System ROM selection */
  if (type >= FILETYPE_MAX)
  {
    /* Save current ROM browser */
    offset_ = offset;
    maxfiles_ = maxfiles;
    selection_ = selection;

    /* Initialize ROM browser */
    maxfiles = ParseDirectory();
    selection = offset = 0;
    while (selection < maxfiles)
    {
      if (strstr(config.sys_rom[type-FILETYPE_MAX], filelist[selection].filename))
      {
        offset = selection;
        while ((offset > (maxfiles - 10)) && (offset > 0))
        {
          offset--;
        }
        break;
      }
      selection++;
    }

    /* By default, select first file in directory if ROM is not found */
    if (selection >= maxfiles)
    {
      selection = 0;
    }

    /* Set menu title and cartridge label type */
    switch (type - FILETYPE_MAX)
    {
      case 0:
      {
        strcpy(m->title,"Sega CD (USA) BIOS Selection");
        type = FILETYPE_MD;
        break;
      }

      case 1:
      {
        strcpy(m->title,"Mega CD (PAL) BIOS Selection");
        type = FILETYPE_MD;
        break;
      }

      case 2:
      {
        strcpy(m->title,"Mega CD (JAP) BIOS Selection");
        type = FILETYPE_MD;
        break;
      }

      case 3:
      {
        strcpy(m->title,"Mega Drive/Genesis BootROM Selection");
        type = FILETYPE_MD;
        break;
      }

      case 4:
      {
        strcpy(m->title,"Master System (USA) BootROM Selection");
        type = FILETYPE_MS;
        break;
      }

      case 5:
      {
        strcpy(m->title,"Master System (PAL) BootROM Selection");
        type = FILETYPE_MS;
        break;
      }

      case 6:
      {
        strcpy(m->title,"Master System (JAP) BootROM Selection");
        type = FILETYPE_MS;
        break;
      }

      case 7:
      {
        strcpy(m->title,"Game Gear BootROM Selection");
        type = FILETYPE_GG;
        break;
      }

      case 8:
      {
        strcpy(m->title,"Game Genie ROM Selection");
        type = FILETYPE_MD;
        break;
      }

      case 9:
      {
        strcpy(m->title,"Action Replay (Pro) ROM Selection");
        type = FILETYPE_MD;
        break;
      }

      case 10:
      {
        strcpy(m->title,"Sonic & Knuckles ROM Selection");
        type = FILETYPE_MD;
        break;
      }

      case 11:
      {
        strcpy(m->title,"Sonic 2 & Knuckles ROM Selection");
        type = FILETYPE_MD;
        break;
      }
    }
  }
  else
  {
    /* reset menu title */
    strcpy(m->title, "Game Selection");
  }

  /* Set default helper for Back button */
  strcpy(m->helpers[0]->comment, "Exit");

  /* Check if we are not browsing recent file list */
  if (type >= 0)
  {
    /* Select cartridge type */
    bg_filesel[9 + type].data = Cart_png[type];
    bg_filesel[9 + type].state |= IMAGE_VISIBLE;

    /* Get current directory */
    char *dir = GetCurrentDirectory();

    /* Get current directory name length */
    int size = strlen(dir);

    /* Check if we are not in Root directory */
    if (size > 1)
    {
      if (dir[size - 2] != ':')
      {
        /* Update helper for Back button */
        strcpy(m->helpers[0]->comment, "Previous Directory");
      }
    }

    /* Initialize directory icon */
    dir_icon.texture = gxTextureOpenPNG(Browser_dir_png,0);
    dir_icon.w = dir_icon.texture->width;
    dir_icon.h = dir_icon.texture->height;
    dir_icon.x = 26;
    dir_icon.y = (26 - dir_icon.h)/2;
  }

  /* Initialize selection bar */
  bar_over.texture = gxTextureOpenPNG(Overlay_bar_png,0);
  bar_over.w = bar_over.texture->width;
  bar_over.h = bar_over.texture->height;
  bar_over.x = 16;
  bar_over.y = (26 - bar_over.h)/2;

  /* Initialize Menu */
  GUI_InitMenu(m);
  string_offset = 0;

  while (1)
  {
    /* ROM file snapshot/database */
    if (old != selection)
    {
      /* close any screenshot texture first */
      gxTextureClose(&bg_filesel[8].texture);
      bg_filesel[8].state &= ~IMAGE_VISIBLE;

      old = selection;
      string_offset = 0;

      if (!filelist[selection].flags)
      {
        /* recent game list -> variable game types */
        if (type < 0)
        {
          /* check if game type has changed */
          type = history.entries[selection].filetype;
          if (!(bg_filesel[9 + type].state & IMAGE_VISIBLE))
          {
            /* hide all cartridge labels */
            for (i=0; i<FILETYPE_MAX; i++)
            {
              gxTextureClose(&bg_filesel[9 + i].texture);
              bg_filesel[9 + i].state &= ~IMAGE_VISIBLE;
            }

            /* open cartridge label texture */
            bg_filesel[9 + type].texture = gxTextureOpenPNG(Cart_png[type],0);

            /* show selected cartridge label */
            bg_filesel[9 + type].state |= IMAGE_VISIBLE;
          }

          /* default screenshot file path */
          sprintf(fname,"%s/snaps/%s/%s", DEFAULT_PATH, Cart_dir[type], filelist[selection].filename);

          /* restore recent type flag */
          type = -1;
        }
        else
        {
          /*  default screenshot file path */
          sprintf(fname,"%s/snaps/%s/%s", DEFAULT_PATH, Cart_dir[type], filelist[selection].filename);
        }

        /* remove original file extension */
        i = strlen(fname) - 1;
        while ((i > 0) && (fname[i] != '.')) i--;
        if (i > 0) fname[i] = 0;

        /* add PNG file extension */
        strcat(fname, ".png");

        /* try to load screenshot file */
        snap = fopen(fname, "rb");
        if (snap)
        {
          bg_filesel[8].texture = gxTextureOpenPNG(0,snap);
          if (bg_filesel[8].texture)
          {
            bg_filesel[8].state |= IMAGE_VISIBLE;
          }
          fclose(snap);
        }
      }
    }

    /* update Action button helper */
    if (m->selected != -1)
    {
      /* out of focus */
      strcpy(action_select.comment,"");
    }
    else if (filelist[selection].flags)
    {
      /* this is a directory */
      strcpy(action_select.comment,"Open Directory");
    }
    else
    {
      /* this is a ROM file */
      if (offset_ != -1)
      {
        /* System ROM browser */
        strcpy(action_select.comment,"Select File");
      }
      else
      {
        /* Normal ROM browser */
        strcpy(action_select.comment,"Load File");
      }
    }
 
    /* Draw menu*/
    GUI_DrawMenu(m);

#ifdef HW_RVL
    if (Shutdown)
    {
      gxTextureClose(&bar_over.texture);
      gxTextureClose(&dir_icon.texture);
      gxTextureClose(&w_pointer);
      GUI_DeleteMenu(m);
      GUI_FadeOut();
      shutdown();
      SYS_ResetSystem(SYS_POWEROFF, 0, 0);
    }
    else if (m_input.ir.valid)
    {
      /* get cursor position */
      x = m_input.ir.x;
      y = m_input.ir.y;

      /* draw wiimote pointer */
      gxDrawTextureRotate(w_pointer, x-w_pointer->width/2, y-w_pointer->height/2, w_pointer->width, w_pointer->height,m_input.ir.angle,255);

      /* ensure we are in the selectable area */
      if ((x < 380) && (y >= 108) && (y <= 368))
      {
        /* find selected item */
        selection = (y - 108) / 26;
        if (selection > 9) selection = 9;
        selection += offset;
        if (selection >= maxfiles) selection = old;

        /* reset selection */
        m->selected = -1;
      }
      else
      {
        /* disable selection */
        m->selected = m->max_buttons + 2;

        /* find selected button */
        for (i=0; i<2; i++)
        {
          button = m->arrows[i];
          if (button)
          {
            if (button->state & BUTTON_VISIBLE)
            {
              if ((x>=button->x)&&(x<=(button->x+button->w))&&(y>=button->y)&&(y<=(button->y+button->h)))
              {
                m->selected = m->max_buttons + i;
                break;
              }
            }
          }
        }
      }
    }
    else
    {
      /* reset selection */
      m->selected = -1;
    }
#endif

    /* copy EFB to XFB */
    gxSetScreen();

    p = m_input.keys;

    /* highlight next item */
    if (p & PAD_BUTTON_DOWN)
    {
      selection++;
      if (selection >= maxfiles)
        selection = offset = 0;
      else if (selection >= (offset + 10))
        offset++;
    }

    /* highlight previous item */
    else if (p & PAD_BUTTON_UP)
    {
      selection--;
      if (selection < 0)
      {
        selection = maxfiles - 1;
        offset = maxfiles - 10;
        if (offset < 0) offset = 0;
      }
      else if (selection < offset)
        offset --;
    }

    /* go back one page */
    else if (p & (PAD_TRIGGER_L | PAD_BUTTON_LEFT)) 
    {
      if (maxfiles >= 10)
      {
        selection -= 10;
        if (selection < 0)
        {
          /* first page */
          selection = offset = 0;
        }
        else if (selection < offset)
        {
          /* previous page */
          offset -= 10;
          if (offset < 0) offset = 0;
        }
      }
    }

    /* go forward one page */
    else if (p & (PAD_TRIGGER_R | PAD_BUTTON_RIGHT))
    {
      if (maxfiles >= 10)
      {
        selection += 10;
        if (selection > maxfiles - 1)
        {
          /* last page */
          selection = maxfiles - 1;
          offset = maxfiles - 10;
        }
        else if (selection >= (offset + 10))
        {
          /* next page */
          offset += 10;
          if (offset > (maxfiles - 10)) offset = maxfiles - 10;
        }
      }
    }

    /* quit */
    else if (p & PAD_TRIGGER_Z)
    {
      /* System ROM selection */
      if (offset_ != -1)
      {
        /* restore current ROM browser */
        offset = offset_;
        maxfiles = maxfiles_;
        selection = selection_;
      }

      gxTextureClose(&bar_over.texture);
      gxTextureClose(&dir_icon.texture);
      GUI_DeleteMenu(m);
      return -1;
    }

    /* previous directory */
    else if (p & PAD_BUTTON_B)
    {
      string_offset = 0;

      /* recent ROM list */
      if (type < 0)
      {
        /* exit */
        gxTextureClose(&bar_over.texture);
        gxTextureClose(&dir_icon.texture);
        GUI_DeleteMenu(m);
        return -1;
      }

      /* update browser directory (and get current folder)*/
      if (UpdateDirectory(1, prev_folder))
      {
        /* get directory entries */
        maxfiles = ParseDirectory();

        /* Get current directory */
        char *dir = GetCurrentDirectory();

        /* current directory name length */
        int size = strlen(dir);

        /* Adjust helper for back button */
        if ((size > 1) && (dir[size - 2] != ':'))
        {
          /* Back button goes up one directory */
          strcpy(m->helpers[0]->comment, "Previous Directory");
        }
        else
        {
          /* Back button exits from root directory */
          strcpy(m->helpers[0]->comment, "Exit");
        }

        /* clear selection by default */
        selection = offset = 0;
        old = -1;

        /* select previous directory */
        for (i=0; i<maxfiles; i++)
        {
          if ((filelist[i].flags) && !strcmp(prev_folder,filelist[i].filename))
          {
            selection = i;
            while (i  >= (offset + 10))
            {
              offset += 10;
              if (offset > (maxfiles - 10)) offset = maxfiles - 10;
            }
            break;
          }
        }
      }
      else
      {
        /* System ROM selection */
        if (offset_ != -1)
        {
          /* restore current ROM browser */
          offset = offset_;
          maxfiles = maxfiles_;
          selection = selection_;
        }

        /* Exit */
        gxTextureClose(&bar_over.texture);
        gxTextureClose(&dir_icon.texture);
        GUI_DeleteMenu(m);
        return -1;
      }
    }

    /* open selected file or directory */
    else if (p & PAD_BUTTON_A)
    {
      string_offset = 0;

      /* ensure we are in focus area */
      if (m->selected < m->max_buttons)
      {
        if (filelist[selection].flags)
        {
          /* get new directory */
          UpdateDirectory(0, filelist[selection].filename);

          /* get directory entries */
          maxfiles = ParseDirectory();

          /* clear selection by default */
          selection = offset = 0;
          old = -1;
        }
        else
        {
          int ret;

          /* System ROM selection */
          if (offset_ != -1)
          {
            /* return ROM file index */
            ret = selection;

            /* restore current ROM browser */
            offset = offset_;
            maxfiles = maxfiles_;
            selection = selection_;
          }
          else
          {
            /* load ROM file from device then return ROM size (zero if an error occured) */
            ret = LoadFile(selection);
          }

          /* exit menu */
          gxTextureClose(&bar_over.texture);
          gxTextureClose(&dir_icon.texture);
          GUI_DeleteMenu(m);
          return ret;
        }
      }

#ifdef HW_RVL
      /* arrow buttons selected */
      else if (m->selected == m->max_buttons)
      {
        /* up arrow */
        selection--;
        if (selection < 0)
        {
          selection = maxfiles - 1;
          offset = selection - 10 + 1;
        }
        if (selection < offset) offset -= 10;
        if (offset < 0) offset = 0;
      }
      else if (m->selected == (m->max_buttons+1))
      {
        /* down arrow */
        selection++;
        if (selection == maxfiles)
          selection = offset = 0;
        if ((selection - offset) >= 10)
          offset += 10;
      }
#endif
    }
  }
}

void ClearSelector(u32 max)
{
  maxfiles = max;
  offset = 0;
  selection = 0;
}
