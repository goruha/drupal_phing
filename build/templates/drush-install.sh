#!/bin/bash


DRUSH_BINARY="$2"

DRUPAL_ROOT="$1"
DRUPAL_ACC_NAME="${project.drupal.admin.name}"
DRUPAL_ACC_PASS="${project.drupal.admin.password}"
DRUPAL_ACC_MAIL="${project.drupal.admin.mail}"
DRUPAL_LOCALE="${project.drupal.locale}"
DRUPAL_CLEAN_URL="${project.drupal.clean_url}"
DRUPAL_SITE_NAME="${project.drupal.site.name}"
DRUPAL_SITE_MAIL="${project.drupal.site.mail}"
DRUPAL_SITES_SUBDIR="${drupal.site.subdir}"
DRUPAL_PROFILE="${project.drupal.profile}"
DRUPAL_ENVIRONMENT="${env.environment}"

${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" vset maintenance_mode 1 > /dev/null 2>&1

if [ "$?" == "0" ]; then
	echo "site installed"
else
	echo "site not installed"
  commands[1]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --account-name="${DRUPAL_ACC_NAME}" --account-pass="${DRUPAL_ACC_PASS}" --account-mail="${DRUPAL_ACC_MAIL}" --locale="${DRUPAL_LOCALE}" --clean-url="${DRUPAL_CLEAN_URL}" --site-name="${DRUPAL_SITE_NAME}" --site-mail="${DRUPAL_SITE_MAIL}" --sites-subdir="${DRUPAL_SITES_SUBDIR}" --yes site-install ${DRUPAL_PROFILE} install_configure_form.locale="${DRUPAL_LOCALE}"'
fi
commands[2]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" --yes cache-clear drush'
commands[3]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" --yes solution_install'
commands[4]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" --yes --force --strict=0 environment-switch ${DRUPAL_ENVIRONMENT}'
commands[5]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" --yes cache-clear all'
commands[6]='${DRUSH_BINARY} --root="${DRUPAL_ROOT}" --user="1" vset maintenance_mode 0'

for cmd in ${!commands[*]}
do
  eval ${commands[$cmd] 2>$1}
  if [ "$?" == "1" ]; then
    echo "Command ${commands[$cmd]} failed"
    exit $?
  fi
done
