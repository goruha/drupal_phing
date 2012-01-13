api = 2
core = 6.x

;--------------------
; Build Core
;--------------------

projects[] = drupal

;--------------------
; Build Kit overrides
;--------------------

projects[notechaos][subdir] = contrib
projects[rubik][subdir] = contrib


;--------------------
; Additional Contrib
;--------------------
projects[cck][subdir] 		=      contrib/dev
projects[date][subdir] 		=      contrib/dev
projects[cck_time][subdir] 		=      contrib/dev
projects[cck_time][version] 		=      1.x-dev
projects[filefield][subdir] 		=      contrib/dev
projects[imagefield][subdir] 		=      contrib/dev
projects[backreference][subdir] 		=      contrib/dev
projects[backreference][patch][] = "http://drupal.org/files/issues/backreference-n922292-5.patch"
projects[backreference][patch][] = patches/backreference/backreference-features.patch

;--------------------
; Custom
;--------------------


;--------------------
; Development
;--------------------

projects[devel][subdir] = contrib/dev
projects[drush_migrate][subdir] = contrib/dev
projects[drush_migrate][version] 		=      1.x-dev
projects[features][subdir] 		=      contrib/dev
projects[ctools][subdir] 		=      contrib/dev
projects[strongarm][subdir] 		=      contrib/dev
projects[admin_menu][subdir] 		=      contrib/dev
projects[hacked][subdir] 		=      contrib/dev
projects[coder][subdir] 		=      contrib/dev