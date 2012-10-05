drop trigger tcopy on testtrigger;
drop function copytonew();

create or replace function copytonew() returns trigger as $newtest$
    declare
        table_name text;
        val text;
    begin

    table_name := 'newtest';
    val := NEW.string;

    IF(TG_OP='INSERT') THEN
        IF(position('a' in NEW.string) > 0) THEN
            EXECUTE 'INSERT INTO ' || table_name || ' ("string") values (' || quote_literal(val) || ')';
            RETURN NULL;
        END IF;
        END IF;

    RETURN NEW;

    end
$newtest$ LANGUAGE plpgsql;

create trigger tcopy
BEFORE INSERT ON testtrigger
FOR EACH ROW EXECUTE PROCEDURE copytonew();
