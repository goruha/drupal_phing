
<?php

/**
 * @file
 * A Phing task to run Drush commands.
 */
require_once "CommandTask.php";

class HostsAddTask extends CommandTask {

  /**
   * The message passed in the buildfile.
   */
  private $host_file = NULL;
  private $host_name = NULL;
  private $host_ip = NULL;

  /**
   * Path to hosts file
   */
  public function setFile($str) {
    $this->host_file = $str;
  }

  /**
   * Host name
   */
  public function setName($str) {
    $this->host_name = $str;
  }

  /**
   * Host ip
   */
  public function setIp($str) {
    $this->host_ip = $str;
  }


  /**
   * The main entry point method.
   */
  public function main() {
    $reg_exp = "/\b((2[0-5]{2}|1[0-9]{2}|[0-9]{1,2})\.){3}(2[0-5]{2}|1[0-9]{2}|[0-9]{1,2})\b.+{$this->host_name}[^\w]?/";
    try {
      $temp_dir = sys_get_temp_dir();

      $hosts_content = file_get_contents($this->host_file);
      if (!preg_match($reg_exp, $hosts_content)) {
          $hosts_content_add = "{$this->host_ip} {$this->host_name}";
          $this->exec("su -c\"echo $hosts_content_add >> $this->host_file\"");
      }
    }
    catch (Exception $e) {
      throw new BuildException($e->getMessage());
    }
  }
}

