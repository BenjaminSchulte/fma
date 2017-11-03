import PluginUtils from '../plugin/PluginUtils';

export default class ClassMembers {
  /**
   * Constructor
   */
  constructor(name, generator) {
    this.name = name;
    this.generator = generator;
    this.initialized = false;

    this.members = {};
  }

  /**
   * Initialize
   */
  initialize() {
    if (this.initialized) {
      return;
    }

    this.initialized = true;
    this.generator(this);
  }

  /**
   * Returns whether an member exists or not
   */
  has(member) {
    this.initialize();

    return this.members.hasOwnProperty(member);
  }

  /**
   * Returns a member
   */
  get(name, self) {
    this.initialize();

    if (!this.members[name]) {
      throw new Error(`Invalid member ${name} for ${this.name}`);
    }

    const member = this.members[name];
    if (!self.__members) { self.__members = {} }
    if (!self.__members[this.name]) { self.__members[this.name] = {} }

    if (!member.macro) {
      member.macro = PluginUtils.createMacro(name, member.args, member.callback);
    }

    const initializedMembers = self.__members[this.name];
    if (!initializedMembers[name]) {
      initializedMembers[name] = member.macro.withSelf(self);
    }

    return initializedMembers[name];
  }

  /**
   * Adds a callback
   */
  on(name, args, callback) {
    this.members[name] = {
      memberId: ClassMembers.nextMemberId++,
      args: args,
      callback: callback
    }
  }

  /**
   * Returns the members for a class
   */
  static forClass(name, generator) {
    if (!ClassMembers.instances.hasOwnProperty(name)) {
      ClassMembers.instances[name] = new ClassMembers(name, generator);
    }

    return ClassMembers.instances[name];
  }
}

ClassMembers.instances = {};
ClassMembers.nextMemberId = 1;
