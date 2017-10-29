;; Allows to debug the application on runtime
module Debug

  ;; Register to write commands to
  ADDRESS = $420E

  ;; Stores whether the debugger is enabled
  ENABLED = false

  ;; Stops the debugger (only in debug mode)
  macro BRK
    ::BRK if ENABLED
  end

  ;; Invokes the code inside the block only in debug mode
  macro only
    yield if ENABLED
  end

  ;; Logs a message to the emulator
  ;;
  ;; You can add register variables by passing ${A}, ${X}, ${Y}
  ;;
  ;; Known levels are:
  ;;
  ;; - trace
  ;; - debug
  ;; - info (default)
  ;; - warn
  ;; - error
  ;; - fatal
  ;;
  ;; Example usage:
  ;;
  ;;    Debug.log "Register A is ${A}", level: :debug
  macro log(message, level=:info)
    return unless ENABLED

    LEVEL_ID = "info"
    case level
    when :trace
      LEVEL_ID = "TRACE"
    when :debug
      LEVEL_ID = "DEBUG"
    when :info
      LEVEL_ID = "INFO"
    when :warn
      LEVEL_ID = "WARN"
    when :error
      LEVEL_ID = "ERROR"
    when :fatal
      LEVEL_ID = "FATAL"
    else
      raise "Unknown log level '#{level}'. Use trace, debug, info, warn, error, fatal"
    end

    _MSG = "PRINT #{LEVEL_ID} #{message}"
    execute _MSG
  end

  ;; Executes a command
  macro execute(command)
    return unless Debug.ENABLED

    Compiler.print "TODO: Debug.execute"
  end
end
