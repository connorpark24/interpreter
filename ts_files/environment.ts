import { RuntimeVal } from "./values.ts";

export default class Environment {
  private parent?: Environment;
  private variables: Map<string, RuntimeVal>;

  constructor(parnetENV?: Environment) {
    this.parent = parnetENV;
    this.variables = new Map();
  }

  public declareVar(varname: string, value: RuntimeVal): RuntimeVal {
    if (this.variables.has(varname)) {
      throw `Cannot declare variable ${varname} twice`;
    }

    this.variables.set(varname, value);
    return value;
  }

  public assignVar(varname: string, value: RuntimeVal): RuntimeVal {}

  public resolve(varname: string): Environment {
    if (this.variables.has(varname)) {
      return this;
    }

    if (this.parent == undefined) {
      throw `Cannot resolve ${varname}, as it does not exist`;
    }

    return this.parent.resolve(varname);
  }
}