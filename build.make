api = 2
core = 7.x

;--------------------
; Build Core
;--------------------

projects[drupal][version] = 7.21
projects[drupal][patch][] = "patches/drupal/locale_import_overwrite.patch"
projects[drupal][patch][] = "patches/drupal/no_uri_warning.patch"
projects[drupal][patch][] = "patches/drupal/user_list_views_override_break_user_create.patch"
;http://drupal.org/node/1232416
projects[drupal][patch][] = "patches/drupal/core-js-drupal-log-1232416-100-D7.patch"

projects[solution_core][type] = "profile"
projects[solution_core][download][type] = "git"
projects[solution_core][download][url] = "git://github.com/goruha/DrupalSolutionCore7.git"
projects[solution_core][download][branch] = 2.x
projects[solution_core][l10n_path] = "http://ftp.drupal.org/files/translations/7.x/drupal/drupal-7.21.%language.po"
