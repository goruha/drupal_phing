<?php

/**
 * @file
 * A Phing task to run Drush commands.
 */
require_once "CommandTask.php";

class VirtualHostCreateTask extends CommandTask {
  /**
   * The message passed in the buildfile.
   */
  private $sites_avaliable_path = NULL;
  private $host_name = NULL;
  private $host_template_file = NULL;
  private $site_root_path = NULL;
  private $drupal_files_dir = NULL;
  private $sites_enabled_path = NULL;
  private $server_name = NULL;



  /**
   * Path to sites-avaliable dir
   */
  public function setSites_Avaliable_Path($str) {
    $this->sites_avaliable_path = $str;
  }

  /**
   * Host name
   */
  public function setHost_Name($str) {
    $this->host_name = $str;
  }

  /**
   * Host template file
   */
  public function setHost_Template_File($str) {
    $this->host_template_file = $str;
  }

  /**
   * Site root path
   */
  public function setSite_Root_Path($str) {
    $this->site_root_path = $str;
  }

  /**
   * Site root path
   */
  public function setDrupal_Files_Dir($str) {
    $this->drupal_files_dir = $str;
  }

  /**
   * Path to sites-enabled dir
   */
  public function setSites_Enabled_Path($str) {
    $this->sites_enabled_path = $str;
  }

  /**
   * Host name
   */
  public function setServer_Name($str) {
    $this->server_name = $str;
  }


  /**
   * The main entry point method.
   */
  public function main() {
    try {
      $file_name = "{$this->sites_avaliable_path}/{$this->host_name}";

      $temp_dir = sys_get_temp_dir();
      $tmp = tempnam($temp_dir, '');

      $vhost_content = file_get_contents($this->host_template_file);
      $replace_tokens = array('[site_root]', '[host]', '[drupal_files_dir]');
      $replace_values = array( $this->site_root_path, $this->host_name,  $this->drupal_files_dir);
      $vhost_content = str_replace($replace_tokens, $replace_values, $vhost_content);
      file_put_contents($tmp, $vhost_content);

      $this->exec("mv -f $tmp $file_name");

      $enabled_file_name = "{$this->sites_enabled_path}/{$this->host_name}";
      if (!file_exists($enabled_file_name)) {
        $this->exec("ln -s $file_name $enabled_file_name");
        $this->exec("/etc/init.d/$this->server_name restart");
      }
    }
    catch (Exception $e) {
      throw new BuildException($e->getMessage());
    }
  }
}