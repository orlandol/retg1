program RetG1;

type
  Source = object
  public
    constructor Init;
    destructor Done;

    function Open( fileName : String ): Boolean;
    procedure Close;

    function ReadChar: Char;

  protected
    handle : File of Char;

    line : Integer;
    column : Integer;
    nextLine : Integer;
    nextColumn : Integer;

    ch : Char;
    nextCh : Char;
  end;

constructor Source.Init;
begin
end;

destructor Source.Done;
begin
end;

function Source.Open( fileName : String ): Boolean;
begin
  Open := False;

  if length(fileName) > 0 then
  begin
  end;
end;

function Source.ReadChar: Char;
begin
  ReadChar := #0;
end;

procedure Source.Close;
begin
end;

begin
end.
