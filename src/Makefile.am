## Process this file with automake to produce Makefile.in

if BUILD_DLGEDIT
  TOOLS_DIR = tools
endif
SUBDIRS = . modules $(TOOLS_DIR)

bin_PROGRAMS = adonthell-0.3

EXTRA_DIST = .indent.pro prefs.l py_adonthell.i

AM_CPPFLAGS =$(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) $(FT2_CFLAGS)

noinst_LIBRARIES = libadonthell.a

CLEANFILES = $(moddata_SCRIPTS)

headers = \
	achievements.h \
	adonthell.h \
	animation.h \
	audio.h \
	callback.h \
	character_base.h \
	character.h \
	data_screen.h \
	dialog.h \
	dialog_screen.h \
	drawable.h \
	drawing_area.h \
	event.h \
	event_handler.h \
	event_handler_base.h \
	event_list.h \
	fileops.h \
	game.h \
	gamedata.h \
	gamedate.h \
	gametime.h \
	gettext.h \
	image.h \
	input.h \
	label.h \
	label_input.h \
	landmap.h \
	map_event.h \
	map_event_handler.h \
	mapsquare.h \
	mapsquare_walkable.h \
	nls.h \
	mapcharacter.h \
	mapobject.h \
	mapview.h \
	path.h \
	pnm.h \
	prefs.h \
	python_class.h \
	py_callback.h \
	py_object.h \
	quest.h \
	screen.h \
	surface.h \
	storage.h \
	str_hash.h \
	types.h \
	text_bubble.h \
	time_event.h \
	time_event_handler.h \
	win_background.h \
	win_base.h \
	win_border.h \
	win_container.h \
	win_event.h \
	win_font.h \
	win_image.h \
	win_keys.h \
	win_label.h \
	win_manager.h \
	win_mapview.h \
	win_object.h \
	win_scrollbar.h \
	win_scroll.h \
	win_select.h \
	win_theme.h \
	win_ttf.h \
	win_types.h \
	win_wrappers.h \
	win_write.h \
	window.h \
	yarg.h

libadonthell_a_SOURCES = \
	achievements.cc \
	adonthell.cc \
	animation.cc \
	audio.cc \
	character_base.cc \
	character.cc \
	data_screen.cc \
	dialog.cc \
	dialog_screen.cc \
	drawable.cc \
	drawing_area.cc \
	event.cc \
	event_handler.cc \
	event_list.cc \
	fileops.cc \
	game.cc \
	gamedata.cc \
	gamedate.cc \
	gametime.cc \
	image.cc \
	input.cc \
	label.cc \
	label_input.cc \
	landmap.cc \
	lex.prefs.cc \
	map_event.cc \
	map_event_handler.cc \
	mapsquare_walkable.cc \
	mapcharacter.cc \
	mapsquare.cc \
	mapobject.cc \
	mapview.cc \
	nls.cc \
	path.cc \
	pnm.cc \
	prefs.cc \
	python_class.cc \
	py_adonthell_wrap.cc \
	py_callback.cc \
	py_object.cc \
	quest.cc \
	screen.cc \
	surface.cc \
	storage.cc \
	text_bubble.cc \
	time_event.cc \
	time_event_handler.cc \
	win_background.cc \
	win_base.cc \
	win_border.cc \
	win_container.cc \
	win_event.cc \
	win_font.cc \
	win_keys.cc \
	win_manager.cc \
	win_scrollbar.cc \
	win_scroll.cc \
	win_select.cc \
	win_theme.cc \
	win_ttf.cc \
	yarg.cc \
	$(headers)

libadonthell_LDADD = $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS) $(INTLLIBS) $(FT2_LIBS)

adonthell_0_3_LDADD = libadonthell.a $(libadonthell_LDADD)
adonthell_0_3_SOURCES = main.cc

# Note: adonthell.py is also built by this target. 
py_adonthell_wrap.cc : py_adonthell.i $(headers)
	@if test "${SWIG}" != "no" ; then \
	   echo ${SWIG} -python -modern -shadow ${SDL_CFLAGS} -I$(srcdir) -I$(top_srcdir) -c++ -makedefault -o $(srcdir)/$*.cc $(srcdir)/py_adonthell.i; \
	   ${SWIG} -python -modern -shadow ${SDL_CFLAGS} -I$(srcdir) -I$(top_srcdir) -c++ -makedefault -o $(srcdir)/$*.cc $(srcdir)/py_adonthell.i; \
           mv $(srcdir)/adonthell.py $(srcdir)/modules/adonthell.py; \
	else \
	   echo "You need swig >= ${SWIG_MINVER} in order to re-build this file."; \
	   exit 1; \
	fi;

lex.prefs.cc : prefs.l
	flex -olex.prefs.cc $<
