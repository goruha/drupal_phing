api = 2
core = 7.x

;--------------------
; Build Core
;--------------------

;projects[] = drupal
projects[drupal][patch][] = "patches/drupal/locale_import_overwrite.patch"
;projects[drupal][patch][] = "patches/simpletest/simpletest_skipp_support.patch"
projects[drupal][patch][] = "patches/drupal/no_uri_warning.patch"
;http://drupal.org/node/1232416
projects[drupal][patch][] = "patches/drupal/core-js-drupal-log-1232416-100-D7.patch"

;--------------------
; Build Kit overrides
;--------------------



;--------------------
; Additional Contrib
;--------------------
projects[features][version] = "1.0-rc3"
projects[features][subdir] = "contrib"
; http://drupal.org/node/1599188
projects[features][patch][] = "patches/features/features_rc3_empty_drupal_codestyle.patch"
; http://drupal.org/node/1666540
projects[features][patch][] = "patches/features/features_rc3_field_alter_hooks.patch"
projects[features][patch][] = "patches/features/features_permission_non_exited_module.patch"


projects[entityreference][subdir] = "contrib"
projects[entityreference][version] = "1.0-rc3"
projects[entityreference][patch][] = "patches/entityreference/er_rc3_selection_handler_views.patch"
;http://drupal.org/node/1625188
projects[entityreference][patch][] = "http://drupal.org/files/fixed-null-path.patch"
;http://drupal.org/node/1706664
projects[entityreference][patch][] = "http://drupal.org/files/entityreference-fixing_regresion-1625188-4.patch"
;http://drupal.org/node/1665818
projects[entityreference][patch][] = "patches/entityreference/er_rc3_remove_extra_checkplain_for_selectlist.patch"

projects[views][subdir] = "contrib"
; http://drupal.org/node/1565294
projects[views][patch][] = "patches/views/views_revert_no_overrided_views.patch"
projects[views][version] = "3.3"

projects[elysia_cron][subdir] = "contrib"
projects[elysia_cron][version] = "2.1"
;--------------------
; Custom
;--------------------


;--------------------
; Development
;--------------------


projects[solution_core][type] = "profile"
projects[solution_core][download][type] = "git"
projects[solution_core][download][url] = "git://github.com/goruha/DrupalSolutionCore7.git"
projects[solution_core][l10n_path] = "http://ftp.drupal.org/files/translations/7.x/drupal/drupal-7.11.%language.po"


projects[uuid][subdir] = "contrib"
projects[uuid][version] = "1.0-alpha3"
;http://drupal.org/node/1666224
projects[uuid][patch][] = "patches/uuid/uuid.tokes.inc.patch"
;http://drupal.org/node/1666220
projects[uuid][patch][] = "patches/uuid/entity_references_fix.patch"
; http://drupal.org/node/1666218#comment-6180136
projects[uuid][patch][] = "patches/uuid/naming_conflict.patch"

projects[admin_menu][version] = "3.0-rc3"
projects[admin_menu][subdir] = "contrib/dev"
projects[admin_menu][patch][] = "patches/admin_menu/admin_menu_font.patch"

projects[simpletest_clone][subdir] = "contrib/dev"
projects[simpletest_clone][version] = "1.0-beta1"
; http://drupal.org/node/1448042
projects[simpletest_clone][patch][] = "patches/simpletest_clone/simpletest_clone_d712_0.patch"
; http://drupal.org/node/1666566
projects[simpletest_clone][patch][] = "patches/simpletest_clone/simpletest_drupal_reset_static.patch"
; http://drupal.org/node/1666560
projects[simpletest_clone][patch][] = "patches/simpletest_clone/path_variabales.patch"
; http://drupal.org/node/1666560#comment-6180972
projects[simpletest_clone][patch][] = "patches/simpletest_clone/vars_refresh.patch"

projects[selenium][type] = "module"
projects[selenium][subdir] = "contrib/dev"
projects[selenium][download][type] = "git"
projects[selenium][download][url] = "git://github.com/maksmi/seleniumSimpletest.git"
projects[drupal][patch][] ="patches/drupal/selenium_hmac.patch"


projects[l10n_update][subdir] = "contrib"
projects[l10n_update][version] = "1.0-beta3"
projects[l10n_update][patch][] ="patches/l10n_update/check_module_dir.patch"
projects[l10n_update][patch][] ="patches/l10n_update/module_enable.patch"

projects[globalredirect][subdir] = "contrib"
projects[globalredirect][version] = "1.5"
