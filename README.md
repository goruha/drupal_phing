# Drupal phing

Drupal phing is environment for drupal developers
allow to take control over the code and database.
It makes possible real CI and CD for Drupal.

## System requirements

IMPORTANT: Tested only on Ubuntu linux.
Please report about other OS cases.
Windows have a lot of problems.

### Install requirements

	$ pear config-set auto_discover 1
	$ pear channel-discover pear.phing.info
	$ pear install --alldeps phing/phing-beta
	$ pear install --alldeps "channel://pear.php.net/VersionControl_SVN-0.4.0"
	$ pear install --alldeps "channel://pear.php.net/VersionControl_GIT-0.4.4"
	$ pear install --alldeps PHP_CodeSniffer
	$ pear channel-discover pear.phpmd.org
	$ pear channel-discover pear.pdepend.org
	$ pear install --alldeps phpmd/PHP_PMD
	$ pear install pear.phpunit.de/phpcpd
	$ pear channel-discover pear.pdepend.org
	$ pear install pdepend/PHP_Depend-beta
	$ apt-get install subversion
	$ sudo usermod -a -G www-data {user_name}
	relogin

#### Best practice:

	$ ./depends.install.sh
	$ apt-get install subversion
	$ sudo usermod -a -G www-data {user_name}
	relogin

Subversion install from this site
http://subversion.tigris.org/

### Config local environment

Copy default properties example to default.prop environment file

	$ cp build/properties/default.prop.example build/properties/default.prop

#### Best practice:

	$ cp build/properties/default.prop.example build/properties/{developer-name}.prop
	$ cd build/properties
	$ ln -s {developer-name}.prop default.prop

### Install drupal

Just run install phing target to install drupal and host.install target to install host.
	$ phing host.install
	$ phing install

Open drupal page in browser with host you define in local enviroment properties.

## Directory structure

Static directories (Not created in auto mode, stored in git and not cleaned with any phing targets)

	- build // Store build system
	-- properties // Store build system configurations
	--- project.prop // Project specific configurations, are same for different developer environments
	--- default.prop.example // Default example properties for developer specific environments
	--- default.prop // Default properties for developer specific environments. User create with his own
	-- tasks // Store subsystems for build.
	-- templates // Store templates (settings.php, configs apache and nginx, shell script to install drupal on server)
	- capistrano // Store capistration scripts and configurations
	- custom // Store custom code
	-- modules // Store custom modules
	-- themes // Store custom themes
	- drake // Store drush migrate (drake) files. Mostly used to enable/disable modules. http://drupal.org/project/drush_migrate
	- patches // Store patches you need to apply for contrib modules.
	- resources // Store resource files for current project
	-- files // Store drupal files
	build.make // Store drupal make configurations. http://drupal.org/project/drush_make
	build.xml // Core of build system.
	depends.install.sh // Shell script to install pear dependences.

Dynamic directories (Created in automode, store some temporary files, can be cleaned up with phing targets)

	- www // Document root for builded drupal site. Cleaned with $ phing clear
	- tools // Store additional tools required with build system.
	- deploy // Store drupal code ready to deploy to server

## Phing targets

### System

	$ phing -l  // List all avaliable task
	$ phing host.install // Creates virtual host

### Development

	$ phing install // Install drupal from scratch
	$ phing reinstall // Remake code and run updates
	$ phing update  // Run updates
	$ phing remake // Remake code

### Environments

	$ phing development // Set Development environment
	$ phing debug // Set Debug environment
	$ phing staging // Set Staging environment
	$ phing production // Set Production environment

### Solr

	$ phing solr-run // Start solr instance
	$ phing solr-stop // Stop solr instance
	$ phing solr-update-configs // Update solr configs  


## Add module workflow

Add into build.make file

	projects[{module_name}][subdir] = "contrib"
	projects[{module_name}][version] = "{module_version}"

Create drush_migrate script to drake dir.
!IMPORTANT: drush_migrate script name should be {{last_drush_migrate_script} + 1}.drake
Enable module with adding line pm-enable {module_name} to just created drush_migrate script.

### Module dev versions and getting from git

If you use dev version of module, get it directly from git.

	projects[mimemail][subdir] = "contrib" 
	projects[mimemail][download][type] = "git" 
	projects[mimemail][download][url] = "git://git.drupal.org/project/mimemail.git" 
	; 1.x-dev
	projects[mimemail][download][revision] = "c26bb469231abde640d484d2a8faef8f5c3c191f" 
	
To find revision visit 

	http://drupalcode.org/project/{module name}.git
and find there revision for branch you need.

List of revision hash

	$ git ls-remote git://drupalcode.org/project/{module name}.git
	
List of revision hash for branch or tag

	$ git ls-remote git://drupalcode.org/project/{module name}.git branchname\tagname

