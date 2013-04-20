require "#{Dir.pwd}/instances/#{instance}";

role :web, $host

set :application, $app

set :deploy_from, "../deploy"
set :deploy_to, $deploy_path
set :resources_dir, "resources"
set :temp_folder, "tmp"

set :drush_binary, $drush_path

set :deploy_via, :clone
set :use_sudo, false
set :synchronous_connect, true
ssh_options[:auth_methods] = ["publickey"]
set :user, $user

if defined?($gateway)
    set :gateway, $gateway
    ssh_options[:forward_agent] = true
end

set :scm, "git"
set :scm_password, "none"
set :scm_username, "git"
set :git_shallow_clone, 1
set :git_enable_submodules, 1
    set :repository,  "{repository}"
set :git_branch, "stable"
set :keep_releases, 10

namespace :deploy do
    after "deploy:update", "deploy:cleanup"
    after "deploy:update", "deploy:vpndisconnect"

    desc "Definition of repository"
        task :set_repository, :roles => :web do
    end


    desc "prerequisite for the deployment: define and setup the repository"
    before "deploy", :roles => :web do
        deploy.vpnconnect
        deploy.set_repository
        deploy.setup
    end

    desc "VPN connect"
    task :vpnconnect do
        if defined?($vpn)
				run_locally "sudo vpnc {vpn name}"
        end
    end

    desc "VPN disconnect"
    task :vpndisconnect do
        if defined?($vpn)
            run_locally "sudo vpnc-disconnect"
        end
    end

    desc "creation of the deployment directory"
    task :setup, :roles => :web do
        run "umask 02 && mkdir -p #{deploy_to} #{deploy_to}/releases #{deploy_to}/#{resources_dir} #{deploy_to}/#{resources_dir}/backupdb"
    end

    desc "surcharge pour suppression du redemarrage"
    task :restart, :roles => :web do
        vpndisconnect
    end

    desc "Viewing the revision of each release"
    task :show_releases, :roles => :web do
        puts "Unknown with git."
    end

    task :cleanup, :except => { :no_release => true } do
        count = fetch(:keep_releases, 5).to_i
        local_releases = capture("ls -xt #{releases_path}").split.reverse
        if count >= local_releases.length
            logger.important "no old releases to clean up"
        else
            logger.info "keeping #{count} of #{local_releases.length} deployed releases"
            directories = (local_releases - local_releases.last(count)).map { |release|
            File.join(releases_path, release) }.join(" ")

            #fix Drupal-chmod
            directories.each do |one_dir|
                run "chmod ug+w #{one_dir}/sites/default #{one_dir}/sites/default/settings.php #{one_dir}/sites/default/default.settings.php #{one_dir}/sites/default/files"
            end

            try_sudo "rm -rf #{directories}"
        end
    end

    desc "Run Drupal installation script"
    task :drushinstall, :roles => :web do
        on_rollback {
            # rollback db from latest backup
            run "mysql -h#{$db_host} -u#{$db_user} -p#{$db_pass} #{$db_name} < #{$db_backup_path}#{application}.sql", :once => true
        }
        # perform db backup
        run "mysqldump --add-drop-table -h#{$db_host} -u#{$db_user} -p#{$db_pass} #{$db_name} > #{$db_backup_path}#{application}.sql", :once => true
        # perform drush site installation/update procedure
        run "#{release_path}/drush-install.sh #{release_path} #{drush_binary}; echo $?", :once => true
    end

    desc "adaptation to transfer the code in VCS ssh and not directly"
    task :update_code, :roles => :web do
        on_rollback { run "rm -rf #{release_path}" }

        # tar it locally
        system("rm -rf #{deploy_from}/sites/default/files")
        system("rm -rf #{deploy_from}/sites/all/translations")
        system("tar -C #{deploy_from} -c -z -f code_update.tar.gz .")

        # transfer to the remote server
        put(File.read("code_update.tar.gz"), "#{deploy_to}/code_update.tar.gz")

        run "mkdir -p #{release_path}"
        run "tar -C #{release_path} -x -m -z -f #{deploy_to}/code_update.tar.gz"
        run "rm -rf #{deploy_to}/code_update.tar.gz"

        # link configuration / resource / system files
        run "ln -s #{deploy_to}/#{resources_dir}/files #{release_path}/sites/default/files"
        run "ln -s #{deploy_to}/#{resources_dir}/translations #{release_path}/sites/all/translations"

        transaction do
            drushinstall
        end

		#fix Drupal-chmod
		run "chmod ug+w #{release_path}/sites/default #{release_path}/sites/default/settings.php #{release_path}/sites/default/default.settings.php #{release_path}/sites/default/files"

        system("rm -f code_update.tar.gz")
    end

end # namespace deploy

task :theme, :roles => :web do
	  #upload("../resources/html/css/" , "#{current_path}/sites/all/themes/custom/{theme}/", :recursive => true, :via => :scp)
    #upload("../resources/html/js/" , "#{current_path}/sites/all/themes/custom/{theme}/", :recursive => true, :via => :scp)
end

