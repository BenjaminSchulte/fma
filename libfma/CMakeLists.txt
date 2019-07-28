set(LIBFMA_SRC
  ./src/fma/Version.cpp
  ./src/fma/FileMap.cpp
  ./src/fma/output/OutputAdapter.cpp
  ./src/fma/output/FileOutputAdapter.cpp
  ./src/fma/output/DynamicBuffer.cpp
  ./src/fma/core/Boolean.cpp
  ./src/fma/core/Map.cpp
  ./src/fma/core/Buffer.cpp
  ./src/fma/core/Nil.cpp
  ./src/fma/core/Array.cpp
  ./src/fma/core/SerializerBuffer.cpp
  ./src/fma/core/TypedNumber.cpp
  ./src/fma/core/Function.cpp
  ./src/fma/core/Compiler.cpp
  ./src/fma/core/SymbolReference.cpp
  ./src/fma/core/DeserializerBuffer.cpp
  ./src/fma/core/Class.cpp
  ./src/fma/core/ImmediateNumber.cpp
  ./src/fma/core/DataBlock.cpp
  ./src/fma/core/Number.cpp
  ./src/fma/core/Plugin.cpp
  ./src/fma/core/Object.cpp
  ./src/fma/core/Range.cpp
  ./src/fma/core/String.cpp
  ./src/fma/types/Decorator.cpp
  ./src/fma/types/DecoratorContainer.cpp
  ./src/fma/types/Base.cpp
  ./src/fma/types/Class.cpp
  ./src/fma/types/Block.cpp
  ./src/fma/types/Undefined.cpp
  ./src/fma/types/Pointer.cpp
  ./src/fma/types/Macro.cpp
  ./src/fma/types/RootModule.cpp
  ./src/fma/types/InternalValue.cpp
  ./src/fma/types/Decoratable.cpp
  ./src/fma/types/ClassPrototype.cpp
  ./src/fma/types/Object.cpp
  ./src/fma/types/Module.cpp
  ./src/fma/interpret/Interpreter.cpp
  ./src/fma/interpret/ModuleContext.cpp
  ./src/fma/interpret/NestedContext.cpp
  ./src/fma/interpret/InterpreterQueue.cpp
  ./src/fma/interpret/PrototypeContext.cpp
  ./src/fma/interpret/BaseContext.cpp
  ./src/fma/interpret/ParameterList.cpp
  ./src/fma/interpret/BlockContext.cpp
  ./src/fma/interpret/ClassContext.cpp
  ./src/fma/interpret/Result.cpp
  ./src/fma/interpret/ObjectContext.cpp
  ./src/fma/interpret/CallStack.cpp
  ./src/fma/interpret/InstanceContext.cpp
  ./src/fma/interpret/ProjectContext.cpp
  ./src/fma/interpret/StaticMemberContext.cpp
  ./src/fma/interpret/LogWithStack.cpp
  ./src/fma/interpret/NameHintContext.cpp
  ./src/fma/assem/BinaryCodeGeneratorScope.cpp
  ./src/fma/assem/LocalAddressOperand.cpp
  ./src/fma/assem/MemoryAddressOperand.cpp
  ./src/fma/assem/BitMaskOperand.cpp
  ./src/fma/assem/PcRelativeAddressOperand.cpp
  ./src/fma/assem/Instruction.cpp
  ./src/fma/assem/ConstantNumberOperand.cpp
  ./src/fma/assem/RelativeAddressOperand.cpp
  ./src/fma/assem/DirectPageOperand.cpp
  ./src/fma/assem/SymbolReferenceOperand.cpp
  ./src/fma/assem/Operand.cpp
  ./src/fma/assem/StackAddressOperand.cpp
  ./src/fma/assem/BinaryCodeGenerator.cpp
  ./src/fma/ast/RaiseStatement.cpp
  ./src/fma/ast/ReturnStatement.cpp
  ./src/fma/ast/StringLiteral.cpp
  ./src/fma/ast/Node.cpp
  ./src/fma/ast/ImportStatement.cpp
  ./src/fma/ast/ConstantRange.cpp
  ./src/fma/ast/ConstantExpression.cpp
  ./src/fma/ast/NilExpression.cpp
  ./src/fma/ast/DecoratableStatement.cpp
  ./src/fma/ast/ConstantNumber.cpp
  ./src/fma/ast/InlineMacroExpression.cpp
  ./src/fma/ast/AssignExpression.cpp
  ./src/fma/ast/FunctionDeclarationStatement.cpp
  ./src/fma/ast/ArrayExpression.cpp
  ./src/fma/ast/Identifier.cpp
  ./src/fma/ast/DecoratorCallStatement.cpp
  ./src/fma/ast/ExpressionStatement.cpp
  ./src/fma/ast/IfStatement.cpp
  ./src/fma/ast/OperationExpression.cpp
  ./src/fma/ast/ClassDeclarationStatement.cpp
  ./src/fma/ast/LabelStatement.cpp
  ./src/fma/ast/MapExpression.cpp
  ./src/fma/ast/ConstantBoolean.cpp
  ./src/fma/ast/MacroDeclarationStatement.cpp
  ./src/fma/ast/UnaryOperationExpression.cpp
  ./src/fma/ast/Statement.cpp
  ./src/fma/ast/CallExpression.cpp
  ./src/fma/ast/DataBlockDeclarationStatement.cpp
  ./src/fma/ast/IdentifierExpression.cpp
  ./src/fma/ast/ModuleDeclarationStatement.cpp
  ./src/fma/ast/RequireStatement.cpp
  ./src/fma/ast/Parameter.cpp
  ./src/fma/ast/CaseStatement.cpp
  ./src/fma/symbol/ConstantNumber.cpp
  ./src/fma/symbol/SignedAssertRangeReference.cpp
  ./src/fma/symbol/SymbolReference.cpp
  ./src/fma/symbol/CalculatedNumber.cpp
  ./src/fma/Project.cpp
  ./src/fma/main.cpp
  ./src/fma/linker/Linker.cpp
  ./src/fma/linker/LinkerBlock.cpp
  ./src/fma/linker/LinkerObject.cpp
  ./src/fma/Parser.cpp
  ./src/fma/instruct/DynamicBinaryData.cpp
  ./src/fma/instruct/BinaryData.cpp
  ./src/fma/plugin/MemoryPluginAdapter.cpp
  ./src/fma/Reference.cpp
  ./src/fma/File.cpp
  ./src/fma/Log.cpp
  ./src/fma/util/term.cpp
  ./src/fma/util/sha256.cpp
  ./src/fma/serialize/DeserializeInput.cpp
  ./src/fma/serialize/SerializeObject.cpp
  ./src/fma/serialize/DeserializeObject.cpp
  ./src/fma/serialize/SerializeOutput.cpp
  ./src/fma/serialize/SerializerRegistry.cpp
)
set(LIBFMA_HEADERS
  ./include/fma/output/DynamicBuffer.hpp
  ./include/fma/output/OutputAdapter.hpp
  ./include/fma/output/FileOutputAdapter.hpp
  ./include/fma/core/Buffer.hpp
  ./include/fma/core/TypedNumber.hpp
  ./include/fma/core/Number.hpp
  ./include/fma/core/Boolean.hpp
  ./include/fma/core/ImmediateNumber.hpp
  ./include/fma/core/Map.hpp
  ./include/fma/core/Range.hpp
  ./include/fma/core/Function.hpp
  ./include/fma/core/Object.hpp
  ./include/fma/core/Array.hpp
  ./include/fma/core/DataBlock.hpp
  ./include/fma/core/Compiler.hpp
  ./include/fma/core/SerializerBuffer.hpp
  ./include/fma/core/SymbolReference.hpp
  ./include/fma/core/Nil.hpp
  ./include/fma/core/DeserializerBuffer.hpp
  ./include/fma/core/Plugin.hpp
  ./include/fma/core/String.hpp
  ./include/fma/core/Class.hpp
  ./include/fma/types/Pointer.hpp
  ./include/fma/types/Base.hpp
  ./include/fma/types/Block.hpp
  ./include/fma/types/DecoratorContainer.hpp
  ./include/fma/types/Decoratable.hpp
  ./include/fma/types/InternalValue.hpp
  ./include/fma/types/Module.hpp
  ./include/fma/types/Object.hpp
  ./include/fma/types/Macro.hpp
  ./include/fma/types/Decorator.hpp
  ./include/fma/types/RootModule.hpp
  ./include/fma/types/Undefined.hpp
  ./include/fma/types/Class.hpp
  ./include/fma/types/ClassPrototype.hpp
  ./include/fma/interpret/ProjectContext.hpp
  ./include/fma/interpret/ObjectContext.hpp
  ./include/fma/interpret/NameHintContext.hpp
  ./include/fma/interpret/CallStack.hpp
  ./include/fma/interpret/Result.hpp
  ./include/fma/interpret/StaticMemberContext.hpp
  ./include/fma/interpret/ModuleContext.hpp
  ./include/fma/interpret/ClassContext.hpp
  ./include/fma/interpret/InterpreterQueue.hpp
  ./include/fma/interpret/NestedContext.hpp
  ./include/fma/interpret/PrototypeContext.hpp
  ./include/fma/interpret/Interpreter.hpp
  ./include/fma/interpret/ParameterList.hpp
  ./include/fma/interpret/BlockContext.hpp
  ./include/fma/interpret/LogWithStack.hpp
  ./include/fma/interpret/BaseContext.hpp
  ./include/fma/interpret/InstanceContext.hpp
  ./include/fma/interpret/Parameter.hpp
  ./include/fma/assem/LabelInstruction.hpp
  ./include/fma/assem/Operand.hpp
  ./include/fma/assem/RegisterOperand.hpp
  ./include/fma/assem/RelativeAddressOperand.hpp
  ./include/fma/assem/StackAddressOperand.hpp
  ./include/fma/assem/Instruction.hpp
  ./include/fma/assem/ConstantNumberOperand.hpp
  ./include/fma/assem/BinaryCodeGenerator.hpp
  ./include/fma/assem/DirectPageOperand.hpp
  ./include/fma/assem/FlagOperand.hpp
  ./include/fma/assem/BitMaskOperand.hpp
  ./include/fma/assem/SymbolReferenceOperand.hpp
  ./include/fma/assem/BinaryCodeGeneratorScope.hpp
  ./include/fma/assem/PcRelativeAddressOperand.hpp
  ./include/fma/assem/LocalAddressOperand.hpp
  ./include/fma/assem/MemoryAddressOperand.hpp
  ./include/fma/Parser.hpp
  ./include/fma/ast/Expression.hpp
  ./include/fma/ast/Statement.hpp
  ./include/fma/ast/Node.hpp
  ./include/fma/ast/Identifier.hpp
  ./include/fma/symbol/CalculatedNumber.hpp
  ./include/fma/symbol/SignedAssertRangeReference.hpp
  ./include/fma/symbol/Reference.hpp
  ./include/fma/symbol/SymbolReference.hpp
  ./include/fma/symbol/ConstantNumber.hpp
  ./include/fma/parser/ParserHeading.hpp
  ./include/fma/Error.hpp
  ./include/fma/linker/LinkerObject.hpp
  ./include/fma/linker/LinkerBlock.hpp
  ./include/fma/linker/Linker.hpp
  ./include/fma/Project.hpp
  ./include/fma/Reference.hpp
  ./include/fma/Log.hpp
  ./include/fma/File.hpp
  ./include/fma/instruct/Rtl.hpp
  ./include/fma/instruct/Jns.hpp
  ./include/fma/instruct/Decw.hpp
  ./include/fma/instruct/Movi.hpp
  ./include/fma/instruct/Rti.hpp
  ./include/fma/instruct/Bit.hpp
  ./include/fma/instruct/Subw.hpp
  ./include/fma/instruct/Decjnz.hpp
  ./include/fma/instruct/Jo.hpp
  ./include/fma/instruct/Nop.hpp
  ./include/fma/instruct/Rts.hpp
  ./include/fma/instruct/Ror.hpp
  ./include/fma/instruct/Jge.hpp
  ./include/fma/instruct/Call.hpp
  ./include/fma/instruct/Push.hpp
  ./include/fma/instruct/Rol.hpp
  ./include/fma/instruct/Brk.hpp
  ./include/fma/instruct/Inc.hpp
  ./include/fma/instruct/Andnot.hpp
  ./include/fma/instruct/Or.hpp
  ./include/fma/instruct/Jbc.hpp
  ./include/fma/instruct/Jno.hpp
  ./include/fma/instruct/Jc.hpp
  ./include/fma/instruct/Js.hpp
  ./include/fma/instruct/Incw.hpp
  ./include/fma/instruct/Movw.hpp
  ./include/fma/instruct/DynamicBinaryData.hpp
  ./include/fma/instruct/Xchg.hpp
  ./include/fma/instruct/Asl.hpp
  ./include/fma/instruct/Jnz.hpp
  ./include/fma/instruct/Lsr.hpp
  ./include/fma/instruct/Jnc.hpp
  ./include/fma/instruct/Cmp.hpp
  ./include/fma/instruct/Div.hpp
  ./include/fma/instruct/Stp.hpp
  ./include/fma/instruct/And.hpp
  ./include/fma/instruct/Mov.hpp
  ./include/fma/instruct/Wai.hpp
  ./include/fma/instruct/Jz.hpp
  ./include/fma/instruct/Cmpjne.hpp
  ./include/fma/instruct/Not.hpp
  ./include/fma/instruct/BinaryData.hpp
  ./include/fma/instruct/Pop.hpp
  ./include/fma/instruct/Adc.hpp
  ./include/fma/instruct/Dec.hpp
  ./include/fma/instruct/Jl.hpp
  ./include/fma/instruct/Mul.hpp
  ./include/fma/instruct/Jmp.hpp
  ./include/fma/instruct/Addw.hpp
  ./include/fma/instruct/Eor.hpp
  ./include/fma/instruct/Sbc.hpp
  ./include/fma/instruct/Jbs.hpp
  ./include/fma/plugin/BinaryGeneratorPluginAdapter.hpp
  ./include/fma/plugin/MemoryPluginAdapter.hpp
  ./include/fma/plugin/Plugin.hpp
  ./include/fma/Version.hpp
  ./include/fma/util/term.hpp
  ./include/fma/util/sha256.hpp
  ./include/fma/FileMap.hpp
  ./include/fma/serialize/SerializeObject.hpp
  ./include/fma/serialize/SerializeHeader.hpp
  ./include/fma/serialize/DeserializeObject.hpp
  ./include/fma/serialize/DeserializeInput.hpp
  ./include/fma/serialize/SerializeTypes.hpp
  ./include/fma/serialize/SerializeOutput.hpp
  ./include/fma/serialize/SerializerRegistry.hpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


set(CMAKE_CXX_STANDARD 11)

set(ICU_DEBUG ON)

add_definitions(-DFMA_VERSION=\"1.0.0\")

find_package(Boost REQUIRED COMPONENTS program_options filesystem locale thread)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

bison_target(FmaParser grammar/fma.y ${CMAKE_CURRENT_BINARY_DIR}/fma.y.cpp VERBOSE)
flex_target(FmaScanner grammar/fma.l ${CMAKE_CURRENT_BINARY_DIR}/fma.l.cpp)
add_flex_bison_dependency(FmaScanner FmaParser)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/fma.l.cpp PROPERTIES COMPILE_FLAGS -Wno-sign-compare)

add_library(libfma SHARED ${LIBFMA_SRC} ${BISON_FmaParser_OUTPUTS} ${FLEX_FmaScanner_OUTPUTS})
set_target_properties(libfma PROPERTIES VERSION 1.0.0)
set_target_properties(libfma PROPERTIES SOVERSION 1)
#set_target_properties(libfma PROPERTIES PUBLIC_HEADER "${LIBFMA_HEADERS}")
target_include_directories(libfma PRIVATE ${Boost_INCLUDE_DIRS} ${ICU_INCLUDE_DIRS})
target_include_directories(libfma PUBLIC include)
target_link_libraries(libfma ${Boost_LIBRARIES} ${FLEX_LIBRARIES} ${ICU_LIBRARIES})

configure_file(libfma.pc.in libfma.pc @ONLY)
install(TARGETS libfma LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fma)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/fma DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libfma/libfma.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

