// $Id$

ICON MODULE
-----------

This module is a Google Summer of Code project that adds a central system for
icons in Drupal, which will help improve the usability. In a way similar to
themes or modules, it allow users to install icon sets and choose which icons
to use for their themes.

By taking advantage of this module, developers may use icons in their modules
in an easy and coherent way, without reinventing the wheel.

The module is not production ready. Please report any issues you come across.

Project's wiki page:
http://groups.drupal.org/node/11066

Project's issue queue:
http://drupal.org/project/issues/icon


INSTALLATION
------------

In addition to this module, you will need the following to give it a try:

 * An icon enabled theme. Download a modified version of Garland here:
   http://kreativmango.no/joakim/sites/default/files/garland_icons.zip

 * An icon set. Download Aesthetica here:
   http://kreativmango.no/joakim/sites/default/files/aesthetica.zip

With the above files downloaded, do the following:

1. Extract the modified version of Garland into your themes folder
   (e.g. sites/all/themes)

2. Fire up your browser and go to the administration overview. The icons you
   see there are the default icons provided by this modified version of
   Garland, added as normal using CSS.

3. Go to admin/build/themes and select either Garland (with icons) or
   Minelli (with icons) as the default theme

4. Create a new directory named "icons" in your sites/* directory
   (e.g. sites/all/icons)

5. Extract the Aesthetica icon set into the newly created icons directory

6. Extract this module into your modules folder (e.g. sites/all/modules)

7. Enable the Icon module from admin/build/modules

8. Enable Aesthetica from the tab for your theme at admin/build/themes/icons

9. Notice how some icons have changed (this is where Aesthetica provides new
   icons) while others remain the same (these are the defaults for the theme)


MORE INFORMATION
----------------

For more information on how the module works, see:
http://kreativmango.no/joakim/blog/demo-and-instructions
