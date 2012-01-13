<?php
/**
 * Created by JetBrains PhpStorm.
 * User: goruha
 * Date: 12/19/11
 * Time: 1:00 AM
 * To change this template use File | Settings | File Templates.
 */

require_once "phing/Task.php";

abstract class CommandTask extends Task {
  protected  $command_prefix = '';

  /**
   * Use sudo
   */
  public function setUse_Sudo($str) {
    $this->command_prefix = $str ? 'sudo' : '';
  }


  protected function exec($command) {
    $this->log("Executing '$this->command_prefix $command'...");
    $output = array();
    exec("$this->command_prefix $command", $output, $return);
    // Collect Drush output for display through Phing's log.
    foreach ($output as $line) {
      $this->log($line);
    }
    // Build fail.
    if ($return != 0) {
      throw new BuildException("Drush exited with code $return");
    }
    return $return != 0;
  }
}
